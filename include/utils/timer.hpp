
/*!
 * \brief
 * \file  timer.h
 */

#if !defined(TIMER_H__)
#define TIMER_H__

//
#include <chrono>
#include <thread>


namespace xtd {

    ///
    /// \brief class used to measure the duration between to points in time,
    ///        i.e. between the start and stop point
    ///
    struct timer {
        using high_resolution_clock = std::chrono::high_resolution_clock;
        using time_point = std::chrono::time_point<high_resolution_clock>;

    private:
        time_point start_;

        ///
        /// \brief returns a time point representing the current point in time
        ///
        time_point now() const noexcept {
           return high_resolution_clock::now();
        }

    public:
        ///
        /// \brief start the timer
        ///
        void start() noexcept {
            start_ = now();
		}

        ///
        /// \brief stop the timer
        /// \return the number of time units specified in 'Duration' ellapsed
        ///         since the start
        ///
        template <typename T = std::chrono::milliseconds>
        auto stop() const {
            return std::chrono::duration_cast<T>(now() - start_).count();
		}

        template <typename T = std::chrono::milliseconds>
        auto elapsed() const {
            return std::chrono::duration_cast<T>(now() - start_).count();
        }

        ///
        /// \brief put the current thread to sleep
        /// \param duration 	Time duration to sleep
        ///
        template <typename T = std::chrono::milliseconds>
        static
        void sleep(T duration) {
            std::this_thread::sleep_for(duration);
        }
	};

} // namespace xtd

#endif // TIMER_H__
