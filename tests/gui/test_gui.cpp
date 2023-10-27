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
        painter.setPen(Qt::white);
        painter.drawLine(0, height() / 2, width(), height() / 2);
        painter.drawLine(width() / 2, 0, width() / 2, height());

        // Calculate the position of the red circle after scaling
        int x = width() / 2 + static_cast<int>(xPos * scaleFactor);
        int y = height() / 2 - static_cast<int>(yPos * scaleFactor);

        // Draw the red circle at the origin
        painter.setPen(Qt::red);
        painter.setBrush(Qt::red);
        int radius = 5; // Adjust the radius as needed
        painter.drawEllipse(x - radius, y - radius, 2 * radius, 2 * radius);
    }

public:
    void moveObject(double deltaX, double deltaY) {
        // Move the object along the X and Y axes
        xPos += deltaX;
        yPos += deltaY;

        // Check if the object is out of bounds, and adjust the scaleFactor for zooming
        if (xPos > width() / 2 || yPos > height() / 2) {
            scaleFactor /= 2.0;
        }

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
    window.resize(320, 240);
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
        timer.start(1000); // Adjust the timer interval as needed
    });

    return app.exec();
}
