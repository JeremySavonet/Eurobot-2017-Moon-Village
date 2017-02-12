// Copyright (c) 2016-2017 All Rights Reserved WestBot

#ifndef ANGLEDISTANCE_HPP
#define ANGLEDISTANCE_HPP

#include <stdint.h>

namespace WestBot {

/*!
 * \brief This class offers static method to convert robot representation.
 *        Left/Right <----> Angle/distance
 */
class AngleDistance
{
public:
    /*!
     * \brief Stores the state of two encoders or
     *        pwm in the left / right format.
     */
    struct RsWheels
    {
        int32_t left;
        int32_t right;
    };

    /*!
     * \brief Stores the state of two encoders
     *        or pwm in the distance / angle format.
     */
    struct RsPolar
    {
        int32_t distance;
        int32_t angle;
    };

    /*!
     * \brief Converts the state of two encoders or pwm.
     *
     * This function converts the left / right representation
     * to a angle distance representation.
     *
     * \param [in] w_src The source state, in left / right format.
     * \param [out] p_dst The destination state, in distance / angle format.
     */
    static void rsGetPolarFromWheels( struct RsPolar* p_dst,
                                      struct RsWheels* w_src );

    /*!
     * \brief Converts the state of two encoders or pwm.
     *
     * This function converts the angle / distance representation to
     * an left-right representation.
     *
     * \param [in] p_src The source state in distance / angle format.
     * \param [out] The destination state in left / right format.
     */
    static void rsGetWheelsFromPolar( struct RsWheels* w_dst,
                                      struct RsPolar* p_src );
};

}

#endif // ANGLEDISTANCE_HPP
