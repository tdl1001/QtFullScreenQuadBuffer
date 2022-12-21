#pragma once

#include <QGLWidget>

class GLStereoWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLStereoWidget(QWidget *parent, const QGLFormat &format = QGLFormat::defaultFormat());
	~GLStereoWidget();

	void setFullScreen(bool fullScreen);
	void setInfo(QString text);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	void updateInfo();
	void renderInfo();

private:
	QStringList info;
	bool fullScreen;
};
