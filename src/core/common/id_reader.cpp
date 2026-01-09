/* ------------------------------------------- */
/* Author: Arthur Damböck                      */
/* Date: 2025/2026                             */
/* ------------------------------------------- */

#include "id_reader.h"
#include "../../shared/constants.h"
#include <QFile>
#include <QTextStream>
#include <regex>

using namespace shared;

id_reader::id_reader()
    : m_lines{}, m_file_name{SVG_IDS_PATH} {
    QFile ids_file{QString::fromStdString(m_file_name)};
    if (!ids_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::logic_error{"Error reading SVG IDs: ID file cannot be found!"};
    }

    QTextStream in_txt{&ids_file};
    while (in_txt.atEnd() == false) {
        QString line{in_txt.readLine().trimmed()};
        if (!line.isEmpty()) {
            m_lines.append(line);
        }
    }
    ids_file.close();
}


/* filter the description of the id (last .*_.*_XXXX) */;
QString id_reader::last_id_to_label_name (QString const & id, int uc_cnt){
    const QStringList parts = id.split('_', Qt::SkipEmptyParts);
    const long long start = std::max(0ll, (long long)(parts.size() - uc_cnt));
    QString value = parts.mid(start).join(' ');

    const bool is_number = ! value.isEmpty() &&
                     std::find_if(value.begin(), value.end(),
                                                      [](QChar c){
                            return !c.isDigit();}) == value.end();

    return  is_number ?   value : value.toUpper();
}


QStringList id_reader::get_postfix_components_for_id(QString const & id){
    QStringList result = id.split('_', Qt::SkipEmptyParts);
    result.removeFirst();
    return result;
}

QStringList id_reader::get_results_for(std::string const& regex_) {
    if (!(m_old_regex_.empty()) && (regex_ == m_old_regex_)) {
        return m_old_result;
    }

    QStringList results{};
    const std::regex rgx{regex_};

    for (QString& e : m_lines) {
        if (std::regex_match(e.toStdString(), rgx)) {
            results.append(e);
        }
    }
    m_old_regex_ = regex_;
    m_old_result = results;
    return results;
}

QStringList id_reader::get_results_for(std::string const& regex_a, std::string const& regex_b) {
    const std::string rgx{"((" + regex_a + ").*(" + regex_b + ")|(" + regex_b + ").*(" + regex_a + "))"};
    return get_results_for(rgx);
}
