#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtFullScreenQuadBuffer.h"

class GLStereoWidget;

class QtFullScreenQuadBuffer : public QMainWindow
{
    Q_OBJECT

public:
    QtFullScreenQuadBuffer(QWidget *parent = Q_NULLPTR);

public slots:
void toggleFullScreen(bool checked);
void toggleStereo(bool checked);

protected:
	bool eventFilter(QObject *obj, QEvent *ev);

private:
	void createStereoWidget(bool stereo = false);

private:
    Ui::QtFullScreenQuadBufferClass ui;
	GLStereoWidget* stereoWidget;
};
