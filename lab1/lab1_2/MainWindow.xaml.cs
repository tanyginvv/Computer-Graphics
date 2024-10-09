using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace lab1_2
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private bool isDragging;
        private Point startPoint;

        public void YourWindow()
        {
            InitializeComponent();
        }

        private void Grid_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                isDragging = true;
                startPoint = e.GetPosition(canvas);
                canvas.CaptureMouse();
            }
        }

        private void Grid_MouseMove(object sender, MouseEventArgs e)
        {
            if (isDragging)
            {
                Point currentPoint = e.GetPosition(canvas);
                double deltaX = currentPoint.X - startPoint.X;
                double deltaY = currentPoint.Y - startPoint.Y;

                foreach (UIElement child in canvas.Children)
                {
                    Canvas.SetLeft(child, Canvas.GetLeft(child) + deltaX);
                    Canvas.SetTop(child, Canvas.GetTop(child) + deltaY);
                }

                startPoint = currentPoint;
            }
        }

        private void Grid_MouseUp(object sender, MouseButtonEventArgs e)
        {
            if (e.ChangedButton == MouseButton.Left)
            {
                isDragging = false;
                canvas.ReleaseMouseCapture();
            }
        }
    }
}
