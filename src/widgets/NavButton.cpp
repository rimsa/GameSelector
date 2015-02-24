#include <QEvent>
#include <QGraphicsEffect>
#include <QPropertyAnimation>
#include <QApplication>

#include <GameSelector/widgets/NavButton.h>

NavButton::NavButton(QWidget* parent) :
    QPushButton(parent),
    m_effect(new QGraphicsOpacityEffect(this)),
    m_animation(new QPropertyAnimation(this)) {

    this->installEventFilter(this);

    m_animation->setTargetObject(m_effect);
    m_animation->setPropertyName("opacity");
    m_animation->setDuration(500);
    m_animation->setEasingCurve(QEasingCurve::OutQuad);

    m_effect->setOpacity(0.0);
    this->setGraphicsEffect(m_effect);
    this->setAttribute(Qt::WA_WState_ExplicitShowHide, false);
}

NavButton::~NavButton() {
}

bool NavButton::eventFilter(QObject* watched, QEvent* event) {
    switch (event->type()) {
        case QEvent::Enter:
            if (QPushButton* button = static_cast<QPushButton*>(watched))
                button->setEnabled(true);

            break;
        case QEvent::Leave:
            if (QPushButton* button = static_cast<QPushButton*>(watched))
                button->setEnabled(false);

            break;
        default:
            break;
    }

    return QPushButton::eventFilter(watched, event);
}

void NavButton::showEvent(QShowEvent* event) {
    QPushButton::showEvent(event);

    this->setAttribute(Qt::WA_WState_ExplicitShowHide, false);

    // Call phaseIn directly.
    QMetaObject::invokeMethod(this, "phaseIn", Qt::DirectConnection);
}

void NavButton::hideEvent(QHideEvent* event) {
    QPushButton::hideEvent(event);

    this->setAttribute(Qt::WA_WState_ExplicitShowHide, false);

    // Force widget reshow.
    this->show();

    // Call phaseOut queued, to be executed after show.
    QMetaObject::invokeMethod(this, "phaseOut", Qt::QueuedConnection);
}

void NavButton::phaseIn() {
    // If it is running, stop it.
    m_animation->stop();

    if (m_effect->opacity() <= 0.99) {
        // Start the animation.
        m_animation->setStartValue(m_effect->opacity());
        m_animation->setEndValue(1.0);
        m_animation->start();
    }

    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
}

void NavButton::phaseOut() {
    // If it is running, stop it.
    m_animation->stop();

    if (m_effect->opacity() >= 0.01) {
        // Start the animation.
        m_animation->setStartValue(m_effect->opacity());
        m_animation->setEndValue(0.0);
        m_animation->start();
    }

    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
}
