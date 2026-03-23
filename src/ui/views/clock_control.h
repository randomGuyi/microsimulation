
#ifndef MICROSIMULATION_CLOCK_CONTROL_H
#define MICROSIMULATION_CLOCK_CONTROL_H

#include <QFrame>

namespace gui::views {
  class clock_control : public QWidget{
  Q_OBJECT

  public:
    clock_control(QWidget * parent = nullptr);
  signals : 
    void next_phase_btn();
    void prev_phase_btn();
    void man_mode_btn();
    void auto_mode_btn();
    void start_btn();
    void stop_btn();
    void slider_changed();
  };
} // namespace gui::views
#endif // MICROSIMULATION_CLOCK_CONTROL_H
