#ifdef QPL_USE_BOOST
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#endif


#include <qpl/system.hpp>
#include <qpl/time.hpp>
#include <fstream>
#include <thread>
#include <string>
#include <conio.h>
#include <qpl/winsys.hpp>
#include <qpl/string.hpp>

namespace qpl {

#ifdef QPL_USE_BOOST
    int qpl::execute(const qpl::string_view& what) {
        qpl::println(what);
        //qpl::begin_benchmark();
        boost::asio::io_context ios;
        boost::process::ipstream out;
        boost::process::ipstream err;
        auto child = boost::process::child(what.data(), boost::process::std_out > out, boost::process::std_err > err, ios);
        std::thread out_thread{ [&]() {
            std::string line;
            while (std::getline(out, line))
            {
                std::cout << line << '\n';
            }
        } };
        std::thread err_thread{ [&]() {
            std::string line;
            while (std::getline(out, line))
            {
                std::cout << line << '\n';
            }
        } };
        ios.run();
        child.wait();
        out_thread.join();
        err_thread.join();
        //qpl::println("\n ====== took ", qpl::end_benchmark().string());
        return child.exit_code();
    }


    int qpl::execute(const qpl::string_view& what, const input_t& input) {
        //qpl::begin_benchmark();
        std::mutex mu;
        boost::process::opstream in;
        boost::process::ipstream out;
        boost::process::ipstream err;
        auto child = boost::process::child(what.data(), boost::process::std_out > out, boost::process::std_err > err, boost::process::std_in < in);
        std::thread out_thread{ [&]() {
            std::string line;
            while (std::getline(out, line))
            {
                std::lock_guard guard{ mu };
                std::cout << line << '\n';
            }
        } };
        std::thread err_thread{ [&]() {
            std::string line;
            while (std::getline(out, line))
            {
                std::lock_guard guard{ mu };
                std::cout << line << '\n';
            }
        } };
        std::thread in_thread{ [&]() {
            auto vec = qpl::split(input.string);
            for (auto& e : vec) {
                std::lock_guard guard{ mu };
                in << e << std::endl;
            }
        } };
        child.wait();
        out_thread.join();
        err_thread.join();
        in_thread.join();
        //qpl::println(what, "\ntook ", qpl::end_benchmark().string());
        return child.exit_code();
    }
    int qpl::execute(const qpl::string_view& path, const std::vector<std::string>& arguments) {
        auto s = qpl::to_string(path, ' ', qpl::container_to_string(arguments, " ", ""));
        return qpl::execute(s.c_str());
    }

    int qpl::execute(const qpl::string_view& path, const qpl::args_t& arguments) {
        return qpl::execute(qpl::to_string(path, ' ', arguments.string));
    }
    int qpl::execute(const qpl::string_view& path, const qpl::args_t& arguments, const input_t& input) {
        return qpl::execute(qpl::to_string(path, ' ', arguments.string), input);
    }
#endif


    void qpl::hide_input(bool enable) {
#ifdef WIN32
        HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;
        GetConsoleMode(hStdin, &mode);

        if (!enable)
            mode &= ~ENABLE_ECHO_INPUT;
        else
            mode |= ENABLE_ECHO_INPUT;

        SetConsoleMode(hStdin, mode);

#else
        struct termios tty;
        tcgetattr(STDIN_FILENO, &tty);
        if (!enable)
            tty.c_lflag &= ~ECHO;
        else
            tty.c_lflag |= ECHO;

        (void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
    }

    std::string get_input() {
        std::string result;
        std::getline(std::cin, result);
        return result;
    }
    std::string get_hidden_input(const qpl::string_view& replace) {
        std::string result;
        char c;
        while (true) {
            c = _getch();
            if (c == '\r') {
                break;
            }
            else if (c == (char)8) {
                if (!result.empty()) {
                    qpl::print("\b \b");
                    result.pop_back();
                }
            }
            else {
                qpl::print(replace);
                result += c;
            }
        }
        qpl::println();
        return result;
    }


    void system_pause(qpl::u32 max_keyboard_latency_hz) {
        qpl::print("press enter to continue . . . ");
        if (!std::cin.good()) {
            std::cin.clear();
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        auto t2 = std::chrono::high_resolution_clock::now();

        double delta_t = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1e9;
        if (delta_t < 1.0 / max_keyboard_latency_hz) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    void system_pause(const qpl::string_view& message, qpl::u32 max_keyboard_latency_hz) {
        qpl::print("press enter to ", message, " . . . ");
        if (!std::cin.good()) {
            std::cin.clear();
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        auto t2 = std::chrono::high_resolution_clock::now();

        double delta_t = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1e9;
        if (delta_t < 1.0 / max_keyboard_latency_hz) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    void qpl::system_pause_clear_console(qpl::u32 max_keyboard_latency_hz) {
        qpl::system_pause(max_keyboard_latency_hz);
        qpl::clear_console();
    }
}