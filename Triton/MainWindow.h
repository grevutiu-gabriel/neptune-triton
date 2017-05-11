//
// File:   MainWindow.h
// Author: M.R.Eggleston
//
// Created March 2013
//

#pragma once

#ifdef NOT_MOC
#include <boost/shared_ptr.hpp>
#endif
#include <QImage>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollArea>
#include <QSpinBox>
#include <QTabWidget>
#include <QTextEdit>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <boost/scoped_ptr.hpp>
#include "Picture.h"
#include "Summary.h"

class MainWindow : public QTabWidget
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_width_spin(int value);
    void on_save_as_button();
    void on_control_button();
    void on_browse_button();
    void on_state_changed();
    void on_progress_changed(double fraction);
    void on_final_status();

private:
    bool valid_file(const QString& str);
    void process_file(const QString& str);
    void file_not_valid(const QString& file_name);
    void file_exists(const QString& file_name);
    void save_failed(const QString& file_name);
    void set_image_size(const QString& size);
    void set_new_image_size(const QString& size);
    void set_base_filename(const QString& str);
    void set_output_filename();
    void enable_widgets();
    bool save();
    void change_state(Picture::STATE);

    void setup_ui_layout();
    void connect_signals();

    void dragEnterEvent(QDragEnterEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);
    void dropEvent(QDropEvent* event);

    // Widgets
    QLineEdit *m_input_line;
    QPushButton *m_browse_button;
    QLineEdit *m_output_line;
    QSpinBox *m_width_spin;
    QLabel *m_old_size_label;
    QLabel *m_new_size_label;
    QLabel *m_old_precision_label;
    QLabel *m_new_precision_label;
    QLabel *m_illustration;
    QLabel *m_status_label;
    QPushButton *m_save_as_button;
    QPushButton *m_control_button;
    QProgressBar *m_progress_bar;
    QTextEdit *m_summary_text;
    QScrollArea* m_scrolled;

    boost::shared_ptr<Summary> m_summary;
    boost::scoped_ptr<Picture> m_picture;
    QString m_base_out_file_name;
    Picture::STATE m_state;
    unsigned int m_width;
    QImage m_image;
    bool m_drop_ok;
};

