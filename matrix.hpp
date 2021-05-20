#if !defined (_MATRIX_H__)
#define _MATRIX_H__

#include <iostream>
#include <vector>

template <typename T>
struct matrix {
    using value_type = T;
    using size_type = std::size_t;
    using line_type = std::vector<value_type>;

private:
    std::vector<line_type> m_;
    size_type rows_;
    size_type columns_;

public:
    matrix() = delete;
    matrix(size_type rows, size_type columns) :
        rows_(rows),
        columns_(columns)
    {
        m_.resize(rows_);
        for (auto i = 0u; i < rows_; ++i) {
            m_[i].resize(columns_);
        }
    }

    matrix(matrix const&) = default;

    matrix& operator= (matrix const&) = default;

    matrix(matrix&&) = default;

    matrix& operator= (matrix&&) = default;

    ~matrix() = default;

    size_type rows() const noexcept {
        return rows_;
    }

    size_type columns() const noexcept {
        return columns_;
    }

    line_type& operator[] (size_type i) {
        return m_[i];
    }

    const line_type& operator[] (size_type i) const {
        return m_[i];
    }

};

template <typename T>
std::ostream& operator<< (std::ostream& out, matrix<T> const& m) {
    for (auto i = 0u; i < m.rows(); ++i) {
        for (auto j = 0u; j < m.columns(); ++j) {
            out << m[i][j];
        }
        std::cout << "\n";
    }

    return out;
}

#endif