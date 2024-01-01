#pragma once

template <typename T>
class OmegaException
{
public:
    OmegaException(
        std::string str,
        T data,
        const std::source_location & loc = std::source_location::current(),
        std::stacktrace trace = std::stacktrace::current())
    : err_str_(std::move(str))
    , user_data_(std::move(data))
    , location_(loc)
    , backtrace_(trace)
    {
    }

    std::string & what() { return err_str_; }
    const std::string & what() const noexcept { return err_str_; }
    const std::source_location & where() const noexcept { return location_; }
    const std::stacktrace & stack() const noexcept { return backtrace_; }
    T & data() { return user_data_; }
    const T & data() const { return user_data_; }

private:
    std::string err_str_;
    T user_data_;
    const std::source_location location_;
    const std::stacktrace backtrace_;
};

std::ostream & operator << (std::ostream & os, const std::source_location & location)
{
    os << location.file_name() << "("
       << location.line() << ":"
       << location.column() << "), function '"
       << location.function_name() << "'";

    return os;
}

std::ostream & operator << (std::ostream & os, const std::stacktrace & backtrace)
{
    for (const auto & frame : backtrace) {
        os << frame.source_file() << "("
           << frame.source_line() << "):"
           << frame.description() << std::endl;
    }

    return os;
}

/*
// Example
enum ErrorTypes { bad, very_bad };

using ExampleException = OmegaException<ErrorTypes>;

throw ExampleException("Bad Error", very_bad);

catch (const ExampleException & e)
{
    std::cout << "Failed to process with code ("
              << e.data() << "):"
              << e.what() << std::endl
              << e.where() << std::endl;
}

// Output
Failed to process with code (2): Bad Error
/app/example.cpp(76:50), function DoSomething(unsigned int)
*/
