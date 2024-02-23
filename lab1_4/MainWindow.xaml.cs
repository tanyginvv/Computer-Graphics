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

namespace lab1_4
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Point centerPoint = new Point(-1, -1);

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Canvas_MouseDown(object sender, MouseButtonEventArgs e)
        {
            centerPoint = e.GetPosition(canvas);
            DrawCircle(centerPoint, 150); 
        }

        private void DrawCircle(Point center, int radius)
        {
            List<Point> circlePoints = CalculateCirclePoints(center, radius);

            foreach (var point in circlePoints)
            {
                DrawPixel(point);
            }
        }

        private List<Point> CalculateCirclePoints(Point center, int radius)
        {
            List<Point> circlePoints = new List<Point>();

            int x = radius;
            int y = 0;
            int decisionOver2 = 1 - x;

            while (y <= x)
            {
                circlePoints.Add(new Point(center.X + x, center.Y + y));
                circlePoints.Add(new Point(center.X + y, center.Y + x));
                circlePoints.Add(new Point(center.X - y, center.Y + x));
                circlePoints.Add(new Point(center.X - x, center.Y + y));
                circlePoints.Add(new Point(center.X - x, center.Y - y));
                circlePoints.Add(new Point(center.X - y, center.Y - x));
                circlePoints.Add(new Point(center.X + y, center.Y - x));
                circlePoints.Add(new Point(center.X + x, center.Y - y));

                y++;
                if (decisionOver2 <= 0)
                {
                    decisionOver2 += 2 * y + 1;
                }
                else
                {
                    x--;
                    decisionOver2 += 2 * (y - x) + 1;
                }
            }

            return circlePoints;
        }

        private void DrawPixel(Point point)
        {
            Rectangle rect = new Rectangle
            {
                Width = 2,
                Height = 2,
                Fill = Brushes.Red
            };
            Canvas.SetLeft(rect, point.X);
            Canvas.SetTop(rect, point.Y);
            canvas.Children.Add(rect);
            //тоже что в 3.1
        }
    }
}
