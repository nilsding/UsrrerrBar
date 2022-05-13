#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QColorDialog>
#include <QFileDialog>

#include "userbar.h"

#define PROPERTY_COLOUR "usrrerrbar_color"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{parent}
    , ui(new Ui::MainWindow)
    , mUserBar(new UserBar(this))
    , mScenePreview(new QGraphicsScene(this))
{
    ui->setupUi(this);

    ui->qgvPreview->setScene(mScenePreview);

    connect(ui->qleText, &QLineEdit::textChanged, this, &MainWindow::onTextChanged);
    onTextChanged(ui->qleText->text());

    // prefill backdrop combo box
    ui->qcbBackdropType->addItem(tr("Single colour"), UserBar::BackdropType::SingleColour);
    ui->qcbBackdropType->addItem(tr("Gradient"), UserBar::BackdropType::Gradient);
    connect(ui->qcbBackdropType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::backdropTypeChanged);
    ui->qcbBackdropType->setCurrentIndex(1); // Gradient

    // prefill scanlines combo box
    ui->qcbScanlines->addItem(tr("None"), UserBar::Scanlines::None);
    ui->qcbScanlines->addItem(tr("Black"), UserBar::Scanlines::Black);
    ui->qcbScanlines->addItem(tr("Black, close"), UserBar::Scanlines::CloseBlack);
    ui->qcbScanlines->addItem(tr("Black, dotted"), UserBar::Scanlines::Dotted);
    ui->qcbScanlines->addItem(tr("Black, wide"), UserBar::Scanlines::WideBlack);
    ui->qcbScanlines->addItem(tr("White"), UserBar::Scanlines::White);
    ui->qcbScanlines->addItem(tr("White, dotted"), UserBar::Scanlines::WhiteDotted);
    ui->qcbScanlines->addItem(tr("White, wide"), UserBar::Scanlines::WideWhite);
    ui->qcbScanlines->addItem(tr("Horizontal"), UserBar::Scanlines::Horizontal);
    ui->qcbScanlines->addItem(tr("Grid"), UserBar::Scanlines::Grid);
    connect(ui->qcbScanlines, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::scanlinesChanged);
    ui->qcbScanlines->setCurrentIndex(1); // Black

    connect(ui->qcbShine, &QCheckBox::stateChanged, this, &MainWindow::onShineChanged);
    onShineChanged(ui->qcbShine->checkState());

    connect(mUserBar, &UserBar::backdropSingleColourChanged, this, &MainWindow::setBackdropSingleColourButton);
    connect(mUserBar, &UserBar::backdropGradientColour1Changed, this, &MainWindow::setBackdropGradientColour1Button);
    connect(mUserBar, &UserBar::backdropGradientColour2Changed, this, &MainWindow::setBackdropGradientColour2Button);
    setBackdropSingleColourButton(mUserBar->backdropSingleColour());
    setBackdropGradientColour1Button(mUserBar->backdropGradientColour1());
    setBackdropGradientColour2Button(mUserBar->backdropGradientColour2());

    connect(ui->qgbBackdropImage, &QGroupBox::toggled, this, &MainWindow::onImageToggled);
    connect(mUserBar, &UserBar::imageHeightChanged, this, &MainWindow::onImageHeightChanged);

    renderPreview();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::renderPreview()
{
    mScenePreview->clear();
    auto preview = mUserBar->render();
    mScenePreview->addPixmap(preview);
}

void MainWindow::onTextChanged(const QString& text)
{
    mUserBar->setText(text);
    renderPreview();
}

void MainWindow::backdropTypeChanged(int index)
{
    auto val = ui->qcbBackdropType->itemData(index);

    if (!val.isValid())
    {
        return;
    }

    UserBar::BackdropType type = static_cast<UserBar::BackdropType>(val.toInt());

    mUserBar->setBackdropType(type);

    ui->qgbBackdropSingleColour->hide();
    ui->qgbBackdropGradient->hide();

    switch (type)
    {
        case UserBar::BackdropType::SingleColour:
            ui->qgbBackdropSingleColour->show();
            break;
        case UserBar::BackdropType::Gradient:
            ui->qgbBackdropGradient->show();
            break;
    }

    renderPreview();
}

void MainWindow::scanlinesChanged(int index)
{
    auto val = ui->qcbScanlines->itemData(index);

    if (!val.isValid())
    {
        return;
    }

    UserBar::Scanlines scanlines = static_cast<UserBar::Scanlines>(val.toInt());

    mUserBar->setScanlines(scanlines);

    renderPreview();
}

void MainWindow::onShineChanged(int state)
{
    mUserBar->setShine(state == Qt::CheckState::Checked);

    renderPreview();
}

void MainWindow::onImageToggled(bool on)
{
    mUserBar->setImageEnabled(on);

    renderPreview();
}

void MainWindow::onImageHeightChanged(int height)
{
    ui->qsbImageOffsetY->setMaximum(height);
    ui->qhsImageOffsetY->setMaximum(height);
}

static inline void setButtonStyleSheet(QPushButton* qpb, const QColor& color)
{
    qpb->setStyleSheet("background: " + color.name() + ";");
    qpb->setProperty(PROPERTY_COLOUR, color);

    // some widget themes (like Breeze) indicate an active/focused button by
    // changing the background colour
    // --> clear the focus on this widget
    qpb->clearFocus();
}

void MainWindow::setBackdropSingleColourButton(const QColor& color)
{
    setButtonStyleSheet(ui->qpbSingleColour, color);
    renderPreview();
}

void MainWindow::setBackdropGradientColour1Button(const QColor& color)
{
    setButtonStyleSheet(ui->qpbGradientColour1, color);
    renderPreview();
}

void MainWindow::setBackdropGradientColour2Button(const QColor& color)
{
    setButtonStyleSheet(ui->qpbGradientColour2, color);
    renderPreview();
}

void MainWindow::on_qpbSaveImage_clicked()
{
    QString fileName =
        "userbar_"
        + ui->qleText->text()
            .toLower()
            .replace(QRegularExpression("[/\\:*?\"<>| ]"), "_")
        + ".png";

    QFileDialog fd(this);
    fd.setFileMode(QFileDialog::AnyFile);
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.setDirectory(fileName);
    fd.setNameFilter(QString("%1 (*.png)").arg(tr("PNG images")));
    if (!fd.exec())
    {
        return;
    }

    auto fileList = fd.selectedFiles();
    fileName = fileList.first();

    mUserBar->render().save(fileName);
}

void MainWindow::on_qpbSingleColour_clicked()
{
    QColorDialog cd(ui->qpbSingleColour->property(PROPERTY_COLOUR).value<QColor>(), this);
    if (!cd.exec())
    {
        return;
    }

    mUserBar->setBackdropSingleColour(cd.selectedColor());
}

void MainWindow::on_qpbGradientColour1_clicked()
{
    QColorDialog cd(ui->qpbGradientColour1->property(PROPERTY_COLOUR).value<QColor>(), this);
    if (!cd.exec())
    {
        return;
    }

    mUserBar->setBackdropGradientColour1(cd.selectedColor());
}

void MainWindow::on_qpbGradientColour2_clicked()
{
    QColorDialog cd(ui->qpbGradientColour2->property(PROPERTY_COLOUR).value<QColor>(), this);
    if (!cd.exec())
    {
        return;
    }

    mUserBar->setBackdropGradientColour2(cd.selectedColor());
}

void MainWindow::on_qpbChooseImage_clicked()
{
    QFileDialog fd(this);
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setNameFilter(QString("%1 (*.png *.jpg *.jpeg)").arg(tr("Image files")));
    if (!fd.exec())
    {
        return;
    }

    auto fileList = fd.selectedFiles();
    QString fileName = fileList.first();
    mUserBar->setBackdropImageFromFile(fileName);

    renderPreview();
}

void MainWindow::on_qsbImageOffsetX_valueChanged(int value)
{
    mUserBar->setBackdropImageOffsetX(value);
    ui->qhsImageOffsetX->setValue(value);

    renderPreview();
}

void MainWindow::on_qhsImageOffsetX_valueChanged(int value)
{
    ui->qsbImageOffsetX->setValue(value);
}

void MainWindow::on_qsbImageOffsetY_valueChanged(int value)
{
    mUserBar->setBackdropImageOffsetY(value);
    ui->qhsImageOffsetY->setValue(value);

    renderPreview();
}

void MainWindow::on_qhsImageOffsetY_valueChanged(int value)
{
    ui->qsbImageOffsetY->setValue(value);
}
