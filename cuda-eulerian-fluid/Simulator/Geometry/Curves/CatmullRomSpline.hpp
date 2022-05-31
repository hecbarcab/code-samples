#ifndef HF_SIMULATOR_GEOMETRY_CURVES_CATMULLROM_HPP
#define HF_SIMULATOR_GEOMETRY_CURVES_CATMULLROM_HPP

#include <Simulator/Simulator.hpp>

HF_BEGIN_NAMESPACE(HF, Math)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    template <typename _Value, typename _Collection = std::vector<_Value>>
    class CatmullRomSpline
    {
    public:
        using Value      = _Value;
        using Collection = _Collection;

    public:
        /**
         * \brief 
         * \return 
         */
        uint getNumSegments() const
        {
            return _nodes.size() - 1;
        }

        /**
         * \brief 
         * \return 
         */
        uint getNumNodes() const
        {
            return _nodes.size();
        }

        /**
         * \brief 
         * \return 
         */
        const Collection& getNodes() const
        {
            return _nodes;
        }

    public:
        
        /**
         * \brief 
         * \param node 
         */
        void add(const Value& node)
        {
            _nodes.push_back(node);
        }

        /**
         * \brief 
         */
        void clear()
        {
            _nodes.clear();
        }

        /**
         * \brief 
         * \param t 
         * \return 
         */
        Value evaluate(float t) const
        {
            const float q = clamp(t, 0.0f, float(getNumNodes()));

            const uint segment = uint(floor(q));
            const float time = q - segment;

            return evaluateSegment(segment, time);
        }

        /**
         * \brief 
         * \param segment 
         * \param t 
         * \return 
         */
        Value evaluateSegment(uint segment, float t) const
        {
            const int index0 = max(segment - 1, 0);
            const int index1 = segment;
            const int index2 = min(segment + 1, getNumNodes() - 1);
            const int index3 = min(segment + 2u, getNumNodes() - 1);

            return Evaluate(_nodes[index0], _nodes[index1], _nodes[index2], _nodes[index3], t);
        }

        /**
         * \brief 
         * \param t 
         * \return 
         */
        Value evaluateDerivative(float t) const
        {
            const float q = clamp(t, 0.0f, float(getNumNodes()));

            const uint segment = uint(floor(q));
            const float time = q - segment;

            return evaluateSegmentDerivative(segment, time);
        }

        /**
         * \brief 
         * \param segment 
         * \param t 
         * \return 
         */
        Value evaluateSegmentDerivative(uint segment, float t) const
        {
            const int index0 = max(segment - 1, 0);
            const int index1 = segment;
            const int index2 = min(segment + 1, getNumNodes() - 1);
            const int index3 = min(segment + 2u, getNumNodes() - 1);

            return EvaluateDerivative(_nodes[index0], _nodes[index1], _nodes[index2], _nodes[index3], t);
        }

    public:
        /**
         * \brief 
         * \param p1 
         * \param p2 
         * \param p3 
         * \param p4 
         * \param t 
         * \return 
         */
        static Value Evaluate(const Value& p1, const Value& p2, const Value& p3, const Value& p4, float t)
        {
            return 0.5f * ((-p1 + 3.0f * p2 - 3.0f * p3 + p4) * (t * t * t)
                + (2.0f * p1 - 5.0f * p2 + 4.0f * p3 - p4) * (t * t)
                + (-p1 + p3) * t
                + 2.0f * p2);
        }

        /**
         * \brief 
         * \param p1 
         * \param p2 
         * \param p3 
         * \param p4 
         * \param t 
         * \return 
         */
        static Value EvaluateDerivative(const Value& p1, const Value& p2, const Value& p3, const Value& p4, float t)
        {
            return 0.5f * ((-p1 + 3.0f * p2 - 3.0f * p3 + p4) * (3.0f * t * t)
                + (2.0f * p1 - 5.0f * p2 + 4.0f * p3 - p4) * (2.0f * t)
                + (-p1 + p3));
        }

    private:
        Collection _nodes;
    };

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Math)

#endif /* HF_SIMULATOR_GEOMETRY_CURVES_CATMULLROM_HPP */
