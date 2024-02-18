using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;

namespace lab1_3
{
    public partial class MainWindow : Window
    {
        private Point startPoint = new Point(-1, -1);
        private Point endPoint = new Point(-1, -1);

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Canvas_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (startPoint.X == -1 && startPoint.Y == -1)
            {
                startPoint = e.GetPosition(canvas);
            }
            else if (endPoint.X == -1 && endPoint.Y == -1)
            {
                endPoint = e.GetPosition(canvas);
                DrawLine(startPoint, endPoint);
                startPoint = new Point(-1, -1);
                endPoint = new Point(-1, -1);
            }
        }

        private void DrawLine(Point p1, Point p2)
        {
            int x1 = (int)p1.X;
            int y1 = (int)p1.Y;
            int x2 = (int)p2.X;
            int y2 = (int)p2.Y;

            int dx = Math.Abs(x2 - x1);
            int dy = Math.Abs(y2 - y1);
            int sx = (x1 < x2) ? 1 : -1;
            int sy = (y1 < y2) ? 1 : -1;
            int err = dx - dy;

            List<Point> linePoints = new List<Point>();

            while (true)
            {
                linePoints.Add(new Point(x1, y1));
                if (x1 == x2 && y1 == y2)
                    break;
                int e2 = 2 * err;
                if (e2 > -dy)
                {
                    err -= dy;
                    x1 += sx;
                }
                if (e2 < dx)
                {
                    err += dx;
                    y1 += sy;
                }
            }

            foreach (var point in linePoints)
            {
                DrawPixel(point);
            }
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
        }
    }
}