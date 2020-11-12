#pragma once

#include <mutex>
#include <deque>

namespace cpphttp
{
    namespace internal
    {
        template <typename T>
        class ts_fifo
        {
        public:
            bool isEmpty() noexcept
            {
                std::scoped_lock lock(m_mutex);
                return m_deque.size() == 0;
            }

            void push(T &&val) noexcept
            {
                std::scoped_lock lock(m_mutex);
                m_deque.push_back(std::move(val));
            }

            T pop()
            {
                std::scoped_lock lock(m_mutex);
                if(m_deque.size())
                {
                    auto res = std::move(m_deque.front());
                    m_deque.pop_front();
                    return res;
                }
                throw std::logic_error("FIFO is empty");
            }

        private:
            std::mutex m_mutex;
            std::deque<T> m_deque;
        };
    } // namespace internal
} // namespace cpphttp
