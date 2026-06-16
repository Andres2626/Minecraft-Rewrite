
#define MC_LOG_PREFIX "Log"
#include "Log/Log.h"

namespace MC 
{
    static std::shared_ptr<spdlog::logger> m_logger;

	static void get_time_buff(const char *fmt, char *buffer, size_t size)
	{
		time_t now = time(NULL);
		strftime(buffer, size, fmt, localtime(&now));
	}

	int Log::Init(u32t flags, level_enum lv)
	{
        char t_buff[32];
        get_time_buff("%Y-%m-%d_%H-%M-%S.log", t_buff, 32);

        bool sout = BIT_CHK(flags, MC_LOG_STDOUT);
        bool fout = BIT_CHK(flags, MC_LOG_FILE);

        try {
            std::vector<spdlog::sink_ptr> sinks;

            if (sout) {
                auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                console->set_level(lv);
                sinks.push_back(console);
            }
            if (fout) {
                auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>(t_buff, true);
                file->set_level(lv);
                sinks.push_back(file);
            }

            m_logger = std::make_shared<spdlog::logger>("log", sinks.begin(), sinks.end());
            m_logger->set_level(lv);
            m_logger->flush_on(spdlog::level::warn);
            m_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] %^%l%$: %v");
            spdlog::set_default_logger(m_logger);
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            fprintf(stderr, "Error initializing log %s\n", ex.what());
            return 1;
        }
       
        mc_info("conout={} fout={} log_file=./{} verbose={}", sout, fout, fout ? t_buff : "NULL", (u32t)lv);
        return 0;
	}

	int Log::Fini()
	{
        mc_info("finished");
		return 0;
	}

    std::shared_ptr<spdlog::logger>& Log::GetInstance()
    { 
        return m_logger; 
    };
}