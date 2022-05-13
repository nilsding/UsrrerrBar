#pragma once

#include <QObject>
#include <QColor>
#include <QImage>

class QPainter;

class UserBar : public QObject
{
    Q_OBJECT

public:
    enum BackdropType {
        SingleColour,
        Gradient
    };

    enum Scanlines {
        None,
        Black,
        Dotted,
        WhiteDotted,
        WideBlack,
        CloseBlack,
        WideWhite,
        White,
        Horizontal,
        Grid
    };

    Q_PROPERTY(BackdropType backdropType READ backdropType WRITE setBackdropType NOTIFY backdropTypeChanged);
    Q_PROPERTY(Scanlines scanlines READ scanlines WRITE setScanlines NOTIFY scanlinesChanged);
    Q_PROPERTY(const QColor& backdropSingleColour READ backdropSingleColour WRITE setBackdropSingleColour NOTIFY backdropSingleColourChanged);
    Q_PROPERTY(const QColor& backdropGradientColour1 READ backdropGradientColour1 WRITE setBackdropGradientColour1 NOTIFY backdropGradientColour1Changed);
    Q_PROPERTY(const QColor& backdropGradientColour2 READ backdropGradientColour2 WRITE setBackdropGradientColour2 NOTIFY backdropGradientColour2Changed);
    Q_PROPERTY(const QString& text READ text WRITE setText NOTIFY textChanged);
    Q_PROPERTY(bool imageEnabled READ imageEnabled WRITE setImageEnabled NOTIFY imageEnabledChanged);
    Q_PROPERTY(int imageHeight READ imageHeight NOTIFY imageHeightChanged);
    Q_PROPERTY(bool backdropImageOffsetX READ backdropImageOffsetX WRITE setBackdropImageOffsetX NOTIFY backdropImageOffsetXChanged);
    Q_PROPERTY(bool backdropImageOffsetY READ backdropImageOffsetY WRITE setBackdropImageOffsetY NOTIFY backdropImageOffsetYChanged);
    Q_PROPERTY(bool shine READ shine WRITE setShine NOTIFY shineChanged);

    UserBar(QObject* parent = nullptr);
    ~UserBar();

    QPixmap render();

    BackdropType backdropType() const;
    Scanlines scanlines() const;
    const QColor& backdropSingleColour() const;
    const QColor& backdropGradientColour1() const;
    const QColor& backdropGradientColour2() const;
    bool imageEnabled() const;
    int imageHeight() const;
    int backdropImageOffsetX() const;
    int backdropImageOffsetY() const;
    const QString& text() const;
    bool shine() const;

    void setBackdropImageFromFile(const QString& filePath);

public slots:
    void setBackdropType(BackdropType backdropType);
    void setScanlines(Scanlines scanlines);
    void setBackdropSingleColour(const QColor& color);
    void setBackdropGradientColour1(const QColor& color);
    void setBackdropGradientColour2(const QColor& color);
    void setImageEnabled(bool imageEnabled);
    void setBackdropImageOffsetX(int backdropImageOffsetX);
    void setBackdropImageOffsetY(int backdropImageOffsetY);
    void setText(const QString& text);
    void setShine(bool shine);

signals:
    void backdropTypeChanged(BackdropType backdropType);
    void scanlinesChanged(Scanlines scanlines);
    void backdropSingleColourChanged(const QColor& color);
    void backdropGradientColour1Changed(const QColor& color);
    void backdropGradientColour2Changed(const QColor& color);
    void imageEnabledChanged(bool imageEnabled);
    void imageHeightChanged(int imageHeight);
    void backdropImageOffsetXChanged(int backdropImageOffsetX);
    void backdropImageOffsetYChanged(int backdropImageOffsetY);
    void textChanged(const QString& text);
    void shineChanged(bool shine);

private:
    const int WIDTH = 350;
    const int HEIGHT = 19;

    BackdropType mBackdropType = BackdropType::SingleColour;
    Scanlines mScanlines = Scanlines::None;

    QColor mBackdropSingleColour = QColor::fromRgb(111, 168, 220);
    QColor mBackdropGradientColour1 = QColor::fromRgb(49, 49, 48);
    QColor mBackdropGradientColour2 = QColor::fromRgb(180, 179, 179);

    QImage mBackdropImage = QImage();
    bool mImageEnabled = false;
    int mBackdropImageOffsetX = 0;
    int mBackdropImageOffsetY = 0;

    QString mText = QString();

    bool mShine = true;

    void renderBackdrop(QPainter* painter);
    void renderBackdropSingleColour(QPainter* painter);
    void renderBackdropGradient(QPainter* painter);
    void renderBackdropImage(QPainter* painter);
    void renderScanlines(QPainter* painter);
    void renderScanlinesDotted(QPainter* painter, int offsetX, int offsetY, int spacing);
    void renderScanlinesDiagonal(QPainter* painter, int spacing);
    void renderScanlinesHorizontal(QPainter* painter, int offset, int spacing);
    void renderScanlinesVertical(QPainter* painter, int offset, int spacing);
    void renderText(QPainter* painter);
    void renderShine(QPainter* painter);
    void renderBorder(QPainter* painter);
};
