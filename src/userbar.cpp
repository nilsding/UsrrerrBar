#include "userbar.h"

#include <QFontDatabase>
#include <QImage>
#include <QLinearGradient>
#include <QPainter>
#include <QPixmap>

constexpr int OPACITY = 255 * 0.20; // 20% opacity

UserBar::UserBar(QObject* parent)
    : QObject{parent}
{
}

UserBar::~UserBar()
{
}

QPixmap UserBar::render()
{
    QPixmap pix(WIDTH, HEIGHT);
    auto painter = new QPainter(&pix);
    painter->setRenderHint(QPainter::RenderHint::Antialiasing, false);
    painter->fillRect(0, 0, WIDTH, HEIGHT, Qt::magenta);

    renderBackdrop(painter);
    renderBackdropImage(painter);
    renderScanlines(painter);
    renderText(painter);
    renderShine(painter);
    renderBorder(painter);

    painter->end();

    return pix;
}

void UserBar::setBackdropImageFromFile(const QString& filePath)
{
    mBackdropImage = QImage(filePath);

    if (mBackdropImage.width() > WIDTH)
    {
        mBackdropImage = mBackdropImage.scaledToWidth(WIDTH, Qt::TransformationMode::SmoothTransformation);
    }

    emit(imageHeightChanged(imageHeight()));
}

// property getters {{{

UserBar::BackdropType UserBar::backdropType() const
{
    return mBackdropType;
}

UserBar::Scanlines UserBar::scanlines() const
{
    return mScanlines;
}

const QColor& UserBar::backdropSingleColour() const
{
    return mBackdropSingleColour;
}

const QColor& UserBar::backdropGradientColour1() const
{
    return mBackdropGradientColour1;
}

const QColor& UserBar::backdropGradientColour2() const
{
    return mBackdropGradientColour2;
}

bool UserBar::imageEnabled() const
{
    return mImageEnabled;
}

int UserBar::imageHeight() const
{
    return mBackdropImage.height();
}

int UserBar::backdropImageOffsetX() const
{
    return mBackdropImageOffsetX;
}

int UserBar::backdropImageOffsetY() const
{
    return mBackdropImageOffsetY;
}

const QString& UserBar::text() const
{
    return mText;
}

bool UserBar::shine() const
{
    return mShine;
}

/// }}}

// property setters {{{

void UserBar::setBackdropType(UserBar::BackdropType backdropType)
{
    mBackdropType = backdropType;
    emit(backdropTypeChanged(backdropType));
}

void UserBar::setScanlines(UserBar::Scanlines scanlines)
{
    mScanlines = scanlines;
    emit(scanlinesChanged(scanlines));
}

void UserBar::setBackdropSingleColour(const QColor& color)
{
    mBackdropSingleColour = color;
    emit(backdropSingleColourChanged(color));
}

void UserBar::setBackdropGradientColour1(const QColor& color)
{
    mBackdropGradientColour1 = color;
    emit(backdropGradientColour1Changed(color));
}

void UserBar::setBackdropGradientColour2(const QColor& color)
{
    mBackdropGradientColour2 = color;
    emit(backdropGradientColour2Changed(color));
}

void UserBar::setImageEnabled(bool imageEnabled)
{
    mImageEnabled = imageEnabled;
    emit(imageEnabledChanged(imageEnabled));
}

void UserBar::setBackdropImageOffsetX(int backdropImageOffsetX)
{
    mBackdropImageOffsetX = backdropImageOffsetX;
    emit(backdropImageOffsetXChanged(backdropImageOffsetX));
}

void UserBar::setBackdropImageOffsetY(int backdropImageOffsetY)
{
    mBackdropImageOffsetY = backdropImageOffsetY;
    emit(backdropImageOffsetYChanged(backdropImageOffsetY));
}

void UserBar::setText(const QString& text)
{
    mText = text;
    emit(textChanged(text));
}

void UserBar::setShine(bool shine)
{
    mShine = shine;
    emit(shineChanged(shine));
}

/// }}}

// render methods {{{

void UserBar::renderBackdrop(QPainter* painter)
{
    switch (mBackdropType)
    {
        case SingleColour: return renderBackdropSingleColour(painter);
        case Gradient: return renderBackdropGradient(painter);
    }
}

void UserBar::renderBackdropSingleColour(QPainter* painter)
{
    painter->fillRect(0, 0, WIDTH, HEIGHT, mBackdropSingleColour);
}

void UserBar::renderBackdropGradient(QPainter* painter)
{
    QLinearGradient gradient(0, 0, 0, HEIGHT);
    gradient.setColorAt(0, mBackdropGradientColour1);
    gradient.setColorAt(1, mBackdropGradientColour2);
    painter->fillRect(0, 0, WIDTH, HEIGHT, gradient);
}

void UserBar::renderBackdropImage(QPainter* painter)
{
    if (mBackdropImage.isNull())
    {
        return;
    }


    painter->drawImage(mBackdropImageOffsetX, 0, mBackdropImage, 0, mBackdropImageOffsetY);
}

void UserBar::renderScanlines(QPainter* painter)
{
    if (mScanlines == Scanlines::None)
    {
        return;
    }

    // set colour
    switch (mScanlines)
    {
        case Scanlines::Black:
        case Scanlines::Dotted:
        case Scanlines::WideBlack:
        case Scanlines::CloseBlack:
        case Scanlines::Horizontal:
        case Scanlines::Grid:
            painter->setPen(QColor(0, 0, 0, OPACITY));
            break;

        case Scanlines::WhiteDotted:
        case Scanlines::WideWhite:
        case Scanlines::White:
            painter->setPen(QColor(255, 255, 255, OPACITY));
            break;

        default:
            return;
    }

    // dispatch actual rendering
    switch (mScanlines)
    {
        case Scanlines::Black:
        case Scanlines::White:
            return renderScanlinesDiagonal(painter, 2);

        case Scanlines::Dotted:
            return renderScanlinesDotted(painter, 0, 3, 3);

        case Scanlines::WhiteDotted:
            return renderScanlinesDotted(painter, 1, 1, 2);

        case Scanlines::WideBlack:
            return renderScanlinesDiagonal(painter, 9);

        case Scanlines::CloseBlack:
            return renderScanlinesDiagonal(painter, 4);

        case Scanlines::WideWhite:
            return renderScanlinesDiagonal(painter, 6);

        case Scanlines::Horizontal:
            return renderScanlinesHorizontal(painter, 0, 2);

        case Scanlines::Grid:
            renderScanlinesHorizontal(painter, 6, 6);
            renderScanlinesVertical(painter, 6, 6);
            return;

        default:
            return;
    }
}

void UserBar::renderScanlinesDotted(QPainter* painter, int offsetX, int offsetY, int spacing)
{
    for (int x = offsetX; x < WIDTH; x += spacing + 1)
    {
        for (int y = offsetY; y < HEIGHT; y += spacing + 1)
        {
            painter->drawPoint(x, y);
        }
    }
}

void UserBar::renderScanlinesDiagonal(QPainter* painter, int spacing)
{
    // WIDTH + HEIGHT to fill the bar completely
    for (int x = spacing; x < WIDTH + HEIGHT; x += spacing + 1)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            painter->drawPoint(x - y, y);
        }
    }
}

void UserBar::renderScanlinesHorizontal(QPainter* painter, int offset, int spacing)
{
    for (int y = offset; y < HEIGHT; y += spacing + 1)
    {
        painter->drawLine(0, y, WIDTH, y);
    }
}

void UserBar::renderScanlinesVertical(QPainter* painter, int offset, int spacing)
{
    for (int x = offset; x < WIDTH; x += spacing + 1)
    {
        painter->drawLine(x, 0, x, HEIGHT);
    }
}


void UserBar::renderText(QPainter* painter)
{
    // this should be our visitor.ttf:
    QFont font(QFontDatabase::applicationFontFamilies(0).first());
    font.setStyleStrategy(QFont::NoAntialias);
    font.setPixelSize(10);

    // draw the text on a separate QImage so we can read the drawn pixels
    QImage imageText(WIDTH, HEIGHT, QImage::Format_ARGB32);
    imageText.fill(QColor(6, 2, 1, 0));
    auto tmpPainter = new QPainter(&imageText);
    tmpPainter->setFont(font);
    tmpPainter->setPen(Qt::white);
    tmpPainter->drawText(0, 0, WIDTH - 10, HEIGHT, Qt::AlignRight | Qt::AlignVCenter, mText);
    tmpPainter->end();

    // paint the outlines
    painter->setPen(Qt::black);
    for (int x = 0; x < WIDTH; x++)
    {
        for (int y = 0; y < HEIGHT; y++)
        {
            if (imageText.pixel(x, y) == 0xFFFFFFFF) // white pixel => needs outline
            {
                painter->drawRect(x - 1, y - 1, 2, 2);
            }
        }
    }

    // finally, overlay the previously drawn text
    painter->drawImage(0, 0, imageText);
}

void UserBar::renderShine(QPainter* painter)
{
    if (!mShine)
    {
        return;
    }

    painter->setRenderHint(QPainter::RenderHint::Antialiasing, true);

    QColor transparentWhite = QColor(255, 255, 255, OPACITY);
    painter->setBrush(transparentWhite);
    painter->setPen(transparentWhite);
    painter->drawEllipse(0, -10, WIDTH - 1, HEIGHT - 1);

    painter->setRenderHint(QPainter::RenderHint::Antialiasing, false);
}

void UserBar::renderBorder(QPainter* painter)
{
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::black);
    painter->drawRect(0, 0, WIDTH - 1, HEIGHT - 1);
}

/// }}}
