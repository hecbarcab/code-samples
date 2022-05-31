#ifndef HF_SIMULATOR_UTILITY_ASSERTIONS_HPP
#define HF_SIMULATOR_UTILITY_ASSERTIONS_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    class Exception : public std::exception
    {
    protected:
        Exception(const char* file, uint line)
            : _fileName(file)
            , _lineNumber(line)
        {
        }

    protected:
        std::string _fileName;
        uint        _lineNumber;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    class RuntimeException : public Exception
    {
    public:
        RuntimeException(const char* file, uint line, const std::string& args = "")
            : Exception(file, line)
        {
            std::stringstream ss;
            ss << "Runtime exception " << " at " << file << ":" << line << "." << std::endl;
            if (!args.empty()) ss << args << std::endl;

            _message = ss.str();
        }

    public:
        const char* what() const noexcept override { return _message.c_str(); }

    protected:
        std::string _message;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    class CudaException : public Exception
    {
    public:
        CudaException(cudaError_t err, const char* file, uint line, const std::string& args = "")
            : Exception(file, line)
            , _cudaErrorCode(err)
        {
            std::stringstream ss;
            ss << cudaGetErrorName(err) << " at " << file << ":" << line << "." << std::endl;
            ss << cudaGetErrorString(err) << std::endl;
            if (!args.empty()) ss << args << std::endl;

            _message = ss.str();
        }

    public:
        const char* what() const noexcept override { return _message.c_str(); }
        cudaError_t cudaError() const              { return _cudaErrorCode; }

    protected:
        std::string _message;
        cudaError_t _cudaErrorCode;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    #define HF_THROW(MSG)                                                                         \
        throw HF::RuntimeException(__FILE__, __LINE__, MSG)

    #define HF_THROW_TYPE(TYPE, ...)                                                              \
        throw TYPE(__FILE__, __LINE__, __VA_ARGS__)

    #define HF_IGNORE_THROW(EXPR)                                                                 \
        do                                                                                        \
        {                                                                                         \
            try                                                                                   \
            {                                                                                     \
                EXPR;                                                                             \
            }                                                                                     \
            catch (const std::exception& ex)                                                      \
            {                                                                                     \
                std::cerr << ex.what() << std::endl;                                              \
            }                                                                                     \
        }                                                                                         \
        while(false)

    #define HF_HALT_ON_THROW(EXPR)                                                                \
        do                                                                                        \
        {                                                                                         \
            try                                                                                   \
            {                                                                                     \
                EXPR;                                                                             \
            }                                                                                     \
            catch (const std::exception& ex)                                                      \
            {                                                                                     \
                std::cerr << ex.what() << std::endl; std::system("PAUSE");                        \
                std::abort();                                                                     \
            }                                                                                     \
        }                                                                                         \
        while(false)
    
    #define HF_ASSERT(EXPR, MSG)                                                                  \
        do                                                                                        \
        {                                                                                         \
            if (!(EXPR))                                                                          \
                HF_THROW(MSG);                                                                    \
        }                                                                                         \
        while (false)

    #define HF_ASSERT_IS_TRUE(COND, MSG)                                                          \
        HF_ASSERT((COND) == true, MSG)                                                            
                                                                                                  
    #define HF_ASSERT_IS_FALSE(COND, MSG)                                                         \
        HF_ASSERT((COND) == false, MSG)                                                           
                                                                                                  
    #define HF_ASSERT_IS_NULL(COND, MSG)                                                          \
        HF_ASSERT((COND) == nullptr, MSG)                                                         
                                                                                                  
    #define HF_ASSERT_IS_NOT_NULL(COND, MSG)                                                      \
        HF_ASSERT((COND) != nullptr, MSG)                                                         
                                                                                                  
    #define HF_ASSERT_IS_WITHIN_RANGE(VAL, MINVAL, MAXVAL, MSG)                                   \
        HF_ASSERT((VAL) >= (MINVAL) && (VAL) <= (MAXVAL), MSG)                                        
                                                                                                  
    #define HF_ASSERT_ARE_EQUAL(VAL1, VAL2, MSG)                                                  \
        HF_ASSERT((VAL1) == (VAL2), MSG)                                                          
                                                                                                  
    #define HF_CRITICAL_ASSERT(EXPR, MSG)                                                         \
        HF_HALT_ON_THROW(HF_ASSERT(EXPR, MSG))                                                    
                                                                                                  
    #define HF_CRITICAL_ASSERT_IS_TRUE(COND, MSG)                                                 \
        HF_HALT_ON_THROW(HF_ASSERT_IS_TRUE(COND, MSG))                                            
                                                                                                  
    #define HF_CRITICAL_ASSERT_IS_FALSE(COND, MSG)                                                \
        HF_HALT_ON_THROW(HF_ASSERT_IS_FALSE(COND, MSG))                                           
                                                                                                  
    #define HF_CRITICAL_ASSERT_IS_NULL(COND, MSG)                                                 \
        HF_HALT_ON_THROW(HF_ASSERT_IS_NULL(COND, MSG))                                            
                                                                                                  
    #define HF_CRITICAL_ASSERT_IS_NOT_NULL(COND, MSG)                                             \
        HF_HALT_ON_THROW(HF_ASSERT_IS_NOT_NULL(COND, MSG))                                        
                                                                                                  
    #define HF_CRITICAL_ASSERT_IS_WITHIN_RANGE(VAL, MINVAL, MAXVAL, MSG)                          \
        HF_HALT_ON_THROW(HF_ASSERT_IS_WITHIN_RANGE(VAL, MINVAL, MAXVAL, MSG))                         
                                                                                                  
    #define HF_CRITICAL_ASSERT_ARE_EQUAL(VAL1, VAL2, MSG)                                         \
        HF_HALT_ON_THROW(HF_ASSERT_ARE_EQUAL(VAL1, VAL2, MSG))                                    
                                                                                                  
    #define HF_THROW_CUDA(CODE, ...)                                                              \
        throw HF::CudaException(CODE, __FILE__, __LINE__, __VA_ARGS__)                            
                                                                                                  
    #define HF_CATCH_CUDA(EXPR, ...)                                                              \
        do                                                                                        \
        {                                                                                         \
            cudaError_t err = EXPR;                                                               \
                                                                                                  \
            if (err != cudaSuccess)                                                               \
            {                                                                                     \
                HF_THROW_CUDA(err, __VA_ARGS__);                                                  \
            }                                                                                     \
        }                                                                                         \
        while (false)                                                                             
                                                                                                  
    #define HF_CUDA(CODE, ...)                                                                    \
        HF_CATCH_CUDA(cuda ## CODE, __VA_ARGS__)

    //─────────────────────────────────────────────────────────────────────────────────────────────

    
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_ASSERTIONS_HPP */
