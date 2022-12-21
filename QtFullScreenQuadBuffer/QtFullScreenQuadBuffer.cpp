#include "QtFullScreenQuadBuffer.h"

#include "qevent.h"
#include "qmessagebox.h"

#include "GLStereoWidget.h"

QtFullScreenQuadBuffer::QtFullScreenQuadBuffer(QWidget *parent)
	: QMainWindow(parent)
	, stereoWidget(nullptr)
{
	ui.setupUi(this);

	connect(ui.actionFull, &QAction::triggered, this, &QtFullScreenQuadBuffer::toggleFullScreen);
	connect(ui.actionStereo, &QAction::triggered, this, &QtFullScreenQuadBuffer::toggleStereo);

	createStereoWidget();
}

bool QtFullScreenQuadBuffer::eventFilter(QObject *obj, QEvent *ev)
{
	if (obj == stereoWidget)
	{
		if (ev->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(ev);
			if (keyEvent->key() == Qt::Key_F11)
			{
				ui.actionFull->toggle();
				toggleFullScreen(ui.actionFull->isChecked());
				return true;
			}
		}
	}
	return QMainWindow::eventFilter(obj, ev);
}

void QtFullScreenQuadBuffer::toggleFullScreen(bool checked)
{
	//QMessageBox::information(this, tr("F8 pressed"), tr("checked = %0").arg(checked));
	stereoWidget->setFullScreen(checked);
}

void QtFullScreenQuadBuffer::toggleStereo(bool checked)
{
	if (stereoWidget)
	{
		delete stereoWidget;
		stereoWidget = nullptr;
	}

	createStereoWidget(checked);
}

void QtFullScreenQuadBuffer::createStereoWidget(bool stereo)
{
	QGLFormat format = QGLFormat::defaultFormat();
	format.setStereo(stereo);

	stereoWidget = new GLStereoWidget(this, format);
	stereoWidget->setInfo("Press F11 to enter Full Screen");
	setCentralWidget(stereoWidget);
	stereoWidget->installEventFilter(this);
}