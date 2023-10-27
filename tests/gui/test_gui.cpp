#include <QtWidgets>
#include <QInputDialog>
#include <QLineEdit>
#include <QTimer>

class CustomWidget : public QWidget {
public:
    CustomWidget(QWidget* parent = nullptr) : QWidget(parent), xPos(0), yPos(0), scaleFactor(1.0) {}

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        
        // Clear the background with a black color
        painter.fillRect(event->rect(), Qt::black);

        painter.setPen(Qt::white);

        // Calculate the position of the red circle after scaling
        int x = width() / 2 + static_cast<int>(xPos * scaleFactor);
        int y = height() / 2 - static_cast<int>(yPos * scaleFactor);

        // Calculate the actual size of the scene based on the scaleFactor
        int sceneWidth = width() / scaleFactor;
        int sceneHeight = height() / scaleFactor;

        // Calculate the fixed positions of the x and y axes
        int xAxisY = height() / 2;
        int yAxisX = width() / 2;

        // Draw the x and y axes at fixed positions
        painter.drawLine(0, xAxisY, sceneWidth, xAxisY);
        painter.drawLine(yAxisX, 0, yAxisX, sceneHeight);

        // Draw the red circle at the origin
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        int radius = 5; // Adjust the radius as needed
        painter.drawEllipse(x - radius, y - radius, 2 * radius, 2 * radius);

        // Draw ticks on the x and y axes
        painter.setPen(Qt::white);
        int tickLength = 5; // Adjust the tick length as needed

        // X-axis ticks
        for (int i = -sceneWidth / 2; i <= sceneWidth / 2; i += 50) { // Adjust the spacing between ticks
            int tickX = width() / 2 + static_cast<int>(i * scaleFactor);
            painter.drawLine(tickX, xAxisY - tickLength, tickX, xAxisY + tickLength);
        }

        // Y-axis ticks
        for (int i = -sceneHeight / 2; i <= sceneHeight / 2; i += 50) { // Adjust the spacing between ticks
            int tickY = height() / 2 - static_cast<int>(i * scaleFactor);
            painter.drawLine(yAxisX - tickLength, tickY, yAxisX + tickLength, tickY);
        }
    }


public:
    void moveObject(double deltaX, double deltaY) {
        // Calculate new positions after the movement
        double newX = xPos + deltaX;
        double newY = yPos + deltaY;

        // Check if the new positions are out of bounds
        if (qAbs(newX) > width() / (2 * scaleFactor) || qAbs(newY) > height() / (2 * scaleFactor)) {
            // Calculate the scaling factor to keep the object within bounds
            double scale = qMin((width() / (2 * qAbs(newX))), (height() / (2 * qAbs(newY))));

            // Update the scale factor
            scaleFactor *= scale;

            // Update positions with the new scale
            xPos = newX;
            yPos = newY;
        } else {
            xPos = newX;
            yPos = newY;
        }

        update(); // Trigger repaint
    }

    void resetObject() {
        xPos = 0;
        yPos = 0;
        scaleFactor = 1.0;
        update(); // Trigger repaint
    }

private:
    double xPos;
    double yPos;
    double scaleFactor;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CustomWidget window;
    window.setStyleSheet("background-color: black;");
    window.resize(640, 480);
    window.show();
    window.setWindowTitle(QApplication::translate("toplevel", "Top-level widget"));

    // Create a second window for the console
    QDialog consoleDialog;
    consoleDialog.setStyleSheet("background-color: black; color: white;");
    consoleDialog.setWindowTitle(QApplication::translate("toplevel", "Console"));

    QVBoxLayout layout(&consoleDialog);
    QLineEdit input(&consoleDialog);
    input.setPlaceholderText("Enter a function (e.g., y = x) or 'stop' to halt:");
    layout.addWidget(&input);

    consoleDialog.setLayout(&layout);
    consoleDialog.show();

    // Timer to continuously update the position of the red circle
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        // Handle function input here (for now, assume 'y = x')
        QString text = input.text().simplified();
        if (text.toLower() == "stop") {
            timer.stop();
            window.resetObject();
        } else {
            // Parse the function and calculate deltaX and deltaY based on the function
            // Here, we assume a simple linear function "y = x" for demonstration purposes
            double deltaX = 10.0;
            double deltaY = 10.0;
            window.moveObject(deltaX, deltaY);
        }
    });

    QObject::connect(&input, &QLineEdit::returnPressed, [&]() {
        // Start the timer when a new function is entered
        timer.start(100); // Adjust the timer interval as needed
    });

    return app.exec();
}
