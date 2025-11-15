//
// Created by Arthur on 15.11.25.
//

#include "microcodelexer.h"


#include "qdebug.h"

#include <QRegularExpression>
#include <QsciScintilla.h>

MicrocodeLexer::MicrocodeLexer(QObject * parent )
    : QsciLexerCustom{parent}
    , m_source{}
{

}

MicrocodeLexer::~MicrocodeLexer() = default;

const char * MicrocodeLexer::language() const {
    return LANGUAGE_HINT;
}

QString MicrocodeLexer::description(int style) const {
    switch(style){
    case Styles::Default: return    "The default.";
    case Styles::Number: return  	"A number";
    case Styles::Bus : return  	"A BUS identifier ";
    case Styles::Operation: return  	"An ALU operation";
    case Styles::Car: return  	"A CAR command";
    case Styles::Comment: return  	"A comment";
    case Styles::Error: return  	"An error string";
    default: return QString("Unknown style %1").arg(style);
    }

}
/* ORIGINAL
void MicrocodeLexer::styleText(int start, int end) {
    QsciScintilla * sci = qobject_cast<QsciScintilla *>(editor());
    if (!sci) return;

    QString full_text = sci->text();
    m_source = full_text.toStdString();

    msim_scanner scanner{m_source, scanner_mode::raw};
    std::vector<token> tokens = scanner.get_tokens();
    if(tokens.empty()) return;

    startStyling(start);

    for (const token &tkn : tokens) {
        if(tkn.type == token_type::UNKNOWN) return;
        if(tkn.abs_pos + tkn.value.size() <= static_cast<size_t>(start)) continue;
        if(static_cast<int>(tkn.abs_pos) >= end) break;
        //int token_start = std::max(static_cast<int>(tkn.abs_pos), start);
        int length = static_cast<int>(tkn.value.size());

        if(static_cast<int>(tkn.abs_pos) + length > end){
            length = end - tkn.abs_pos;
        }
        startStyling(static_cast<int>(tkn.abs_pos));
        setStyling(length, setStyleForToken(tkn.type));
    }
}
*/
/*
void MicrocodeLexer::styleText(int start, int end) {
    QsciScintilla *sci = qobject_cast<QsciScintilla *>(editor());
    if (!sci) return;

    QString text = sci->text().mid(start, end - start);
    msim_scanner scanner{text.toStdString(), scanner_mode::raw};
    auto tokens = scanner.get_tokens();

    int offset = start; // shift local positions into global buffer space
    for (const auto &tkn : tokens) {
        int token_start = offset + tkn.abs_pos;
        int token_end   = token_start + static_cast<int>(tkn.value.size());

        if (token_end <= start) continue;
        if (token_start >= end) break;

        int clipped_start = std::max(token_start, start);
        int clipped_end   = std::min(token_end, end);
        int length = clipped_end - clipped_start;
        if (length <= 0) continue;

        startStyling(clipped_start);
        setStyling(length, setStyleForToken(tkn.type));
    }
}
*/
void MicrocodeLexer::styleText(int start, int end) {
    QsciScintilla *sci = qobject_cast<QsciScintilla *>(editor());
    if (!sci) return;

    // get the slice
    QString slice = sci->text().mid(start, end - start);
    msim_scanner scanner{slice.toStdString(), false};
    auto tokens = scanner.get_tokens();

    // startStyling at the beginning of the requested range
    startStyling(start);

    int pos = start; // current absolute position we've styled so far

    for (const auto &tkn : tokens) {
        int token_global_start = start + static_cast<int>(tkn.abs_pos);
        int token_global_end = token_global_start + static_cast<int>(tkn.value.size());

        // if token is completely before current styling position, skip
        if (token_global_end <= pos) continue;
        // if token starts after requested end, break
        if (token_global_start >= end) break;

        // clip token to requested range
        int clipped_start = std::max(token_global_start, start);
        int clipped_end = std::min(token_global_end, end);

        // fill gap before token with default style
        if (clipped_start > pos) {
            int gap_len = clipped_start - pos;
            setStyling(gap_len, Styles::Default);
            pos += gap_len;
        }

        int length = clipped_end - clipped_start;
        if (length > 0) {
            int style = setStyleForToken(tkn.type);
            setStyling(length, style);
            pos += length;
        }
    }

    // remaining tail
    if (pos < end) {
        setStyling(end - pos, Styles::Default);
    }
}

int MicrocodeLexer::setStyleForToken(const token_type & token){
    auto it = token_to_syle.find(token);
    if(it == token_to_syle.end()) return Styles::Error;
    return it->second;
}
