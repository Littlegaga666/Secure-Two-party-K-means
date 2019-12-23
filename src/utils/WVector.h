
/**
 * \file    WVector.h
 * \author  Jamie Cui
 * \brief   IMPLEMENTATION OF WVector TEMPLATE
 */
#ifndef BIOSECURE_WVECTOR_H
#define BIOSECURE_WVECTOR_H

#include <cassert>
#include <cstring>

namespace bioutils {

/**
 * WVector is a self-implemented pointer-level Vector which is easy to use
 * @tparam T
 */
    template<typename T>
    class WVector {
    public:
        WVector();

        WVector(size_t rows, size_t cols);

        ~WVector();

        /**
         * Resize WVector with given "rows" and "cols"
         * @warning This function removes all data and re-allocate rows and cols
         * @param rows: the number of rows
         * @param cols: the number of columns
         */
        void resize(size_t rows, size_t cols);

        /**
         * Get the raw pointer to the values (1-dimension)
         * @warning This function is for debugging, it's NOT SECURE
         * @return Template raw pointer
         */
        T *GetRawPtr();

        /**
         * Get the row pointer to the given row
         * @param row_idx: the index of one row
         * @return Template row pointer
         */
        T *GetRowPtr(uint32_t row_idx);

        /**
         * Get the line pointer to the given line
         * @warning This function inside calls transpose matrix
         * @param col_idx: the index of one column
         * @return Template line pointer
         */
        T *GetColPtr(uint32_t col_idx);

        /**
         * Get value in the matrix
         * @param row_idx: the index of one row
         * @param col_idx: the index of one row
         * @return Template value
         */
        T GetValue(uint32_t row_idx, uint32_t col_idx);

        /**
         * Remvoe one column from WVector
         * @param col_idx: the index of one row
         */
        void RemoveCol(uint32_t col_idx);

        /**
         * Remvoe one row from WVector
         * @param row_idx: the index of one row
         */
        void RemoveRow(uint32_t row_idx);

        /**
         * Transpose this WVector
         */
        void Transpose();

        /**
         * Print this WVector
         */
        void Print();

        /**
         * Set the size of WVector, this internally allocates spaces for pointer, so only works right after init
         * @param rows: the number of rows
         * @param cols: the number of columns
         */
        void SetMSize(size_t rows, size_t cols);

        /**
         * Getter for m_cols
         * @return m_cols
         */
        uint32_t getMCols() const;

        /**
         * Getter for m_rows
         * @return m_rows
         */
        uint32_t getMRows() const;

        /**
         * Getter for m_size
         * @return m_size
         */
        size_t getMSize() const;

    private:
        uint32_t m_cols;
        uint32_t m_rows;
        size_t m_size;
        T *data_ptr;
        T *data_t_ptr;

        T *transpose();
    };


///////////////////////////////////////////////////////////
///             IMPLEMENTATION OF WVECTOR               ///
///////////////////////////////////////////////////////////

    template<typename T>
    WVector<T>::WVector() {
        m_rows = 0;
        m_cols = 0;
        m_size = 0;
        data_ptr = nullptr;
        data_t_ptr = nullptr;
    }

    template<typename T>
    WVector<T>::WVector(size_t rows, size_t cols) {
        m_rows = rows;
        m_cols = cols;
        m_size = rows * cols;
        data_t_ptr = nullptr;
        data_ptr = new T[m_size];
        std::fill(data_ptr, data_ptr + m_size, 0);
    }

    template<typename T>
    WVector<T>::~WVector() {
        this->m_rows = 0;
        this->m_cols = 0;
        this->m_size = m_rows * m_cols;
        delete[] data_ptr;
        delete[] data_t_ptr;
    }

    template<typename T>
    inline void WVector<T>::RemoveCol(uint32_t col_idx) {
        assert(col_idx < m_cols);
        Transpose();
        assert(data_ptr != nullptr);
        assert(data_t_ptr != nullptr);
        RemoveRow(col_idx);
        Transpose();
    }

    template<typename T>
    inline void WVector<T>::RemoveRow(uint32_t row_idx) {
        assert(row_idx < m_rows);
        assert(data_ptr != nullptr);
        memcpy(data_ptr + m_cols * row_idx, data_ptr + m_cols * (row_idx + 1),
               sizeof(T) * m_cols * (m_rows - row_idx - 1));
        m_rows--;
        m_size = m_rows * m_cols;
    }

    template<typename T>
    void WVector<T>::SetMSize(size_t rows, size_t cols) {
        m_rows = rows;
        m_cols = cols;
        m_size = m_cols * m_rows;
        data_ptr = new T[m_size];
        std::fill(data_ptr, data_ptr + m_size, 0);
    }

    template<typename T>
    inline T WVector<T>::GetValue(uint32_t row_idx, uint32_t col_idx) {
        assert(data_ptr != nullptr);
        assert(row_idx < m_rows);
        assert(col_idx < m_cols);
        return data_ptr[row_idx * m_cols + col_idx];
    }

    template<typename T>
    inline T *WVector<T>::GetRowPtr(uint32_t row_idx) {
        assert(data_ptr != nullptr);
        assert(row_idx < m_rows);
        return (data_ptr + row_idx * m_cols);
    }

    template<typename T>
    inline T *WVector<T>::GetColPtr(uint32_t col_idx) {
        assert(col_idx < m_cols);
        if (data_t_ptr == nullptr) {
            data_t_ptr = transpose();
        }
        assert(data_t_ptr != nullptr);
        return (data_t_ptr + col_idx * m_rows);
    }

    template<typename T>
    inline void WVector<T>::Transpose() {
        data_t_ptr = transpose();
        assert(data_t_ptr != nullptr);
        assert(data_ptr != nullptr);

        T *pt = data_ptr;
        data_ptr = data_t_ptr;
        data_t_ptr = pt;
        size_t temp = m_rows;
        m_rows = m_cols;
        m_cols = temp;
    }

    template<typename T>
    inline T *WVector<T>::transpose() {
        T *temp = new T[m_size];
        for (std::size_t i = 0; i < m_size; i++) {
            temp[(i % m_cols) * m_rows + i / m_cols] = data_ptr[i];
        }
        return temp;
    }

    template<typename T>
    inline void WVector<T>::Print() {
        std::cout << "Size = [" << m_rows << " X " << m_cols << " = " << m_size << "]" << std::endl;
        for (int i = 0; i < m_size; i++) {
            if (i != 0 && i % m_cols == 0)
                printf("\n");
            printf("%3.f ", static_cast<double>(data_ptr[i]));
        }
        printf("\n");
    }

    template<typename T>
    inline T *WVector<T>::GetRawPtr() {
        return data_ptr;
    }

    template<typename T>
    uint32_t WVector<T>::getMCols() const {
        return m_cols;
    }

    template<typename T>
    uint32_t WVector<T>::getMRows() const {
        return m_rows;
    }

    template<typename T>
    size_t WVector<T>::getMSize() const {
        return m_size;
    }
}
#endif //BIOSECURE_WVECTOR_H
