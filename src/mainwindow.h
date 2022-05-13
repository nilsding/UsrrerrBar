#pragma once

#include <QMainWindow>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class UserBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void renderPreview();

    void onTextChanged(const QString& text);
    void backdropTypeChanged(int index);
    void scanlinesChanged(int index);
    void onShineChanged(int state);
    void onImageToggled(bool on);
    void onImageHeightChanged(int height);

    void setBackdropSingleColourButton(const QColor& color);
    void setBackdropGradientColour1Button(const QColor& color);
    void setBackdropGradientColour2Button(const QColor& color);

    void on_qpbSaveImage_clicked();
    void on_qpbSingleColour_clicked();
    void on_qpbGradientColour1_clicked();
    void on_qpbGradientColour2_clicked();
    void on_qpbChooseImage_clicked();
    void on_qsbImageOffsetX_valueChanged(int value);
    void on_qhsImageOffsetX_valueChanged(int value);
    void on_qsbImageOffsetY_valueChanged(int value);
    void on_qhsImageOffsetY_valueChanged(int value);

private:
    Ui::MainWindow* ui;
    UserBar* mUserBar = nullptr;
    QGraphicsScene* mScenePreview = nullptr;
};
