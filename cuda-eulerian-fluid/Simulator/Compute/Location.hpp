#ifndef HF_SIMULATOR_COMPUTE_LOCATION_HPP
#define HF_SIMULATOR_COMPUTE_LOCATION_HPP

HF_BEGIN_NAMESPACE(HF, Compute)
{
    //─────────────────────────────────────────────────────────────────────────────────────────────

    namespace Location
    {
        /**
         * \brief Tag-like structure designating the host-side. Used for disambiguating kernel
         *        execution calls and to specify the residence location of buffers.
         */
        struct HostTag {};

        /**
         * \brief Tag-like structure designating the device-side. Used for disambiguating kernel
         *        execution calls and to specify the residence location of buffers.
         */
        struct DeviceTag {};

        /**
         * \brief Value designating the host-side.
         */
        constexpr HostTag Host;

        /**
         * \brief Value designating the device-side.
         */
        constexpr DeviceTag Device;
    }

    //─────────────────────────────────────────────────────────────────────────────────────────────
}
HF_END_NAMESPACE(HF, Compute)

#endif /* HF_SIMULATOR_COMPUTE_LOCATION_HPP */
