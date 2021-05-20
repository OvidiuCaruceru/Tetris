
/*
 * \file  stack.hpp
 * \brief template stack implementation (minimal, statically allocated)
 */

#if !defined(STACK_H__)
#define STACK_H__

#include <cstddef>
#include <stdexcept>

namespace xtd {

    ///
    /// \brief stack structure
    ///
    template <
        typename T,
        std::size_t N
        >
    struct stack {
        using value_type = T;
        using reference = value_type&;
        using const_reference = value_type const&;
        using size_type = std::size_t;

    private:
        std::size_t count_;
        value_type arr_[N];

    public:
        stack() :
            count_(0)
        {
        }

        ///
        /// \brief empty
        /// \return
        ///
        bool empty() const noexcept {
            return (0 == count_);
        }

        ///
        /// \brief full
        /// \return
        ///
        bool full() const noexcept {
            return (N == count_);
        }

        ///
        /// \brief Returns the number of elements in the underlying container
        /// \return The number of elements in the container
        ///
        size_type size() const noexcept {
            return count_;
        }

        ///
        /// \brief Pushes the given element value to the end of the stack
        /// \param elem
        ///
        void push(value_type elem) {
            if (full()) {
                throw std::overflow_error("Stack full!");
            }

            arr_[count_] = elem;
            ++count_;
            //count_ += 2;      // useful for unittesting only!!!
        }

        ///
        /// \brief Returns reference to the first element in the stack.
        /// 	   This element will be the first element to be removed
        /// 	   on a call to pop().
        /// \return Reference to the first element.
        ///
        const_reference top() const {
            if (empty()) {
                throw std::underflow_error("Stack empty!");
            }

            return arr_[count_ - 1];
        }

        ///
        /// \brief Removes an element from the front of the stack
        /// \return
        ///
        value_type pop() {
            if (empty()) {
                throw std::underflow_error("Stack empty!");
            }

            --count_;
            return arr_[count_];
        }
    };

} // namespace xtd

#endif // STACK_H__
