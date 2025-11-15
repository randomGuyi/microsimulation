//
// Created by Arthur on 15.11.25.
//

#ifndef MICROSIMULATION_STYLES_H
#define MICROSIMULATION_STYLES_H


/* icons */
#define SVG_ICON_SIZE 64

/* styles */

#define REGISTER_STYLE_SHEET  \
"padding: 0px;"  			  \
"margin: 0px;"				  \
"border: 1px solid grey; "    \
"border-radius: 2px;"		  \
"font-weight: bold;" 		  \
"font-size: 8px; " 		      \
"color: black;"				  \
"background: #f0f0f0;"

#define REGISTER_LABLE_STYLE \
"font-size: 4px;" 			 \
"border: 0px;" 				 \
"margin: 2px;" 				 \
"padding: 0px;"


#define REGISTER_VALUE_STYLE \
"font-size: 4px;" \
"margin: 2px;"


#define BIT_DISPLAY_LABEL \
"color: white;"           \
" font-weight: bold;"     \
" background: transparent;"

#define CLOCK_CYCLE_LABEL_STYLE \
"background: rgba(0, 0, 0, 0.4);" \
"font-weight: bold;"            \
"color: white;"                 \
"font-size: 4pt;"               \
"padding: 1px;"                 \
"margin: 1px;" 					\
"border-radius: 2px;"

#define CLOCK_BTN_MANUAL_AUTO_ENABLE_STYLE \
"background: rgba(255,255,255,1.0);" \
"color: black;"						 \
"font-size: 3pt;"                    \
"padding: 1px;"					     \
"margin: 1px;"                       \
"border-radius: 2px;"

#define CLOCK_BTN_MANUAL_AUTO_DISABLE_STYLE \
"background: rgba(216,218,227,1.0);"  \
"color: black;"						  \
"font-size: 3pt;"                     \
"padding: 1px;"				          \
"margin: 1px;"                       \
"border-radius: 2px;"

#define CLOCK_FREQ_SLIDER_STYLE \
"QSlider::groove:horizontal {" \
    "border: 1px solid #999999;" \
    "height: 2px; /* Set the height (size) of the groove/track */" \
    "background: #cccccc;" \
    "background-radius: 2px;" \
    "margin: 0px 0;" \
    "border-radius: 2px;" \
    "}" \
     \
    "QSlider::handle:horizontal {" \
    "background: orange; /* Blue color for the handle */" \
    "border: 1px solid orange;" \
    "width: 4px; /* Set the width (size) of the handle */" \
    "margin: -2px -1px; /* Centers the handle vertically on the 8px groove */" \
    "border-radius: 2px;" \
    "}" \
     \
    "QSlider::add-page:horizontal {" \
    "background: #999999; /* Darker gray for the part *after* the handle */" \
    "border-radius: 2px;" \
    "margin: 0px;" 			\
    "}" \
     \
    "QSlider::sub-page:horizontal {" \
    "background: #87ceeb; /* Light blue/sky blue for the part *before* the handle */" \
    "border-radius: 2px;" \
    "margin: 0px;" 			\
    "}"



#endif //MICROSIMULATION_STYLES_H