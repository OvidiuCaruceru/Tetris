
/*!
 * \brief
 * \file  random.hpp
 */

#if !defined(RANDOM_H__)
#define RANDOM_H__

///
#include <random>


namespace xtd {

    /*!
     * \brief integers random generator class
     */
    template <typename T>
    struct random {
        using value_type = T;

    private:
        std::random_device rdev_;
        std::mt19937 rng_;
//        std::default_random_engine rng_;
        std::uniform_int_distribution<value_type> dist_;

    public:
        /*!
         * \brief random
         * use the random_device (rdev_) once to seed the random number
         * generator (rng_)
         */
        random(value_type begin, value_type end) :
            rng_(rdev_()),
            dist_(begin, end)
        {
        }

        /*!
         * \brief feed
         * \return
         */
        value_type seed() {
            return dist_(rng_);
        }
    };

} // namespace xtd

#endif // RANDOM_H__
