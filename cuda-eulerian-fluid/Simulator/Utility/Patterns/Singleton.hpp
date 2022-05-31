#ifndef HF_SIMULATOR_UTILITY_SINGLETON_HPP
#define HF_SIMULATOR_UTILITY_SINGLETON_HPP

#include <Simulator/Utility/Assertions.hpp>

HF_BEGIN_NAMESPACE(HF)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <class T>
    struct Singleton
    {
    public:
        Singleton()
        {
            HF_ASSERT(_instance == nullptr, "Only single instance allowed.");
            _instance = static_cast<T*>(this);
        }

        virtual ~Singleton()
        {
            HF_ASSERT(_instance != nullptr, "Unable to destroy non-singleton instance.");
            _instance = nullptr;
        }

        Singleton(const Singleton&)            = delete;
        Singleton(Singleton&&)                 = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton& operator=(Singleton&&)      = delete;

    public:
        static T& instanceRef() { HF_ASSERT(_instance != nullptr, "No singleton instance found."); return *_instance; }
        static T* instancePtr() { return _instance; }

    private:
        static T* _instance;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <class T> T* Singleton<T>::_instance = nullptr;

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF)

#endif /* HF_SIMULATOR_UTILITY_SINGLETON_HPP */
