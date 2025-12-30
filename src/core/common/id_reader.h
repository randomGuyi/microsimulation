// id_reader.h
#ifndef ID_READER_H
#define ID_READER_H

#include <string>
#include <QStringList>
namespace shared {
    /* Singelton !! */
    class id_reader {
    public:

        static id_reader& get_instance() {
            static id_reader instance;
            return instance;
        }
        QStringList get_results_for(std::string const & regex_);
        QStringList get_results_for(std::string const & regex_a, std::string const & regex_b);
        QString last_id_to_label_name (QString const & id, int uc_count = 1);
        QStringList get_postfix_components_for_id(QString const & id);


    private:
        id_reader();
        id_reader(const id_reader &) = delete;
        id_reader & operator=(const id_reader &) = delete;

        QStringList m_lines;
        std::string m_file_name;
        std::string m_old_regex_;
        QStringList m_old_result;
    };
}

#endif // ID_READER_H
