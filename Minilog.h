

#ifndef MINILOG_MINILOG_H
#define MINILOG_MINILOG_H

#include <cstdint>
#include <memory>
#include <string>
#include <iosfwd>
#include <type_traits>

namespace minilog
{
    enum class LogLevel : uint8_t { INFO, WARN, CRIT };

    class MiniLogLine
    {
    public:
        MiniLogLine(LogLevel level, char const * file, char const * function, uint32_t line);
        ~MiniLogLine();

        MiniLogLine(MiniLogLine &&) = default;
        MiniLogLine& operator=(MiniLogLine &&) = default;

        void stringify(std::ostream & os);

        MiniLogLine& operator<<(char arg);
        MiniLogLine& operator<<(int32_t arg);
        MiniLogLine& operator<<(uint32_t arg);
        MiniLogLine& operator<<(int64_t arg);
        MiniLogLine& operator<<(uint64_t arg);
        MiniLogLine& operator<<(double arg);
        MiniLogLine& operator<<(std::string const & arg);

        template < size_t N >
        MiniLogLine& operator<<(const char (&arg)[N])
        {
            encode(string_literal_t(arg));
            return *this;
        }

        template < typename Arg >
        typename std::enable_if < std::is_same < Arg, char const * >::value, MiniLogLine& >::type
        operator<<(Arg const & arg)
        {
            encode(arg);
            return *this;
        }

        template < typename Arg >
        typename std::enable_if < std::is_same < Arg, char * >::value, MiniLogLine& >::type
        operator<<(Arg const & arg)
        {
            encode(arg);
            return *this;
        }

        struct string_literal_t
        {
            explicit string_literal_t(char const * s) : m_s(s) {}
            char const * m_s;
        };

    private:
        char * buffer();

        template < typename Arg >
        void encode(Arg arg);

        template < typename Arg >
        void encode(Arg arg, uint8_t type_id);

        void encode(char * arg);
        void encode(char const * arg);
        void encode(string_literal_t arg);
        void encode_c_string(char const * arg, size_t length);
        void resize_buffer_if_needed(size_t additional_bytes);
        void stringify(std::ostream & os, char * start, char const * const end);

    private:
        size_t m_bytes_used;
        size_t m_buffer_size;
        std::unique_ptr < char [] > m_heap_buffer;  //I guess it is upper than 256
        char m_stack_buffer[256 - 2 * sizeof(size_t) - sizeof(decltype(m_heap_buffer)) - 8 /* Reserved */];
    };

    struct minilog
    {

        bool operator==(MiniLogLine &);
    };

    void set_log_level(LogLevel level);

    bool is_logged(LogLevel level);



    struct NonGuaranteedLogger
    {
        NonGuaranteedLogger(uint32_t ring_buffer_size_mb_) : ring_buffer_size_mb(ring_buffer_size_mb_) {}
        uint32_t ring_buffer_size_mb;
    };


    struct GuaranteedLogger
    {
    };


    void initialize(GuaranteedLogger gl, std::string const & log_directory, std::string const & log_file_name, uint32_t log_file_roll_size_mb);
    void initialize(NonGuaranteedLogger ngl, std::string const & log_directory, std::string const & log_file_name, uint32_t log_file_roll_size_mb);

} // namespace minilog

#define NANO_LOG(LEVEL) minilog::minilog() == minilog::MiniLogLine(LEVEL, __FILE__, __func__, __LINE__)
#define LOG_INFO minilog::is_logged(minilog::LogLevel::INFO) && NANO_LOG(minilog::LogLevel::INFO)
#define LOG_WARN minilog::is_logged(minilog::LogLevel::WARN) && NANO_LOG(minilog::LogLevel::WARN)
#define LOG_CRIT minilog::is_logged(minilog::LogLevel::CRIT) && NANO_LOG(minilog::LogLevel::CRIT)

#endif //MINILOG_MINILOG_H
