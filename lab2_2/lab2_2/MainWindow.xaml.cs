using Microsoft.Win32;
using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace lab2_2
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Point? lastPoint;
        private System.Drawing.Color currentColor = System.Drawing.Color.Black;
        private double imgHeight = 400;
        private double imgWidth = 600;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Canvas_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            lastPoint = e.GetPosition(canvas);
        }

        private void Canvas_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.LeftButton == MouseButtonState.Pressed)
            {
                Point currentPoint = e.GetPosition(canvas);
                DrawLine(lastPoint, currentPoint, currentColor);
                lastPoint = currentPoint;
            }
        }

        private void DrawLine(Point? startPoint, Point endPoint, System.Drawing.Color color)
        {
            if (startPoint != null)
            {
                Line line = new Line();
                line.Stroke = new SolidColorBrush(System.Windows.Media.Color.FromRgb(color.R, color.G, color.B));
                line.StrokeThickness = 2;
                line.X1 = startPoint.Value.X;
                line.Y1 = startPoint.Value.Y;
                line.X2 = endPoint.X;
                line.Y2 = endPoint.Y;

                canvas.Children.Add(line);
            }
        }

        private void New_Click(object sender, RoutedEventArgs e)
        {
            canvas.Children.Clear();
        }

        private void Open_Click(object sender, RoutedEventArgs e)
        {
            canvas.Children.Clear();

            OpenFileDialog openFileDialog = new OpenFileDialog();
            if (openFileDialog.ShowDialog() == true)
            {
                BitmapImage bitmapImage = new BitmapImage(new Uri(openFileDialog.FileName));
                Image image = new Image();
                image.Source = bitmapImage;
                canvas.Children.Add(image);
                image.Height = imgHeight;
                image.Width = imgWidth;
                double canvasWidth = canvas.ActualWidth;
                double canvasHeight = canvas.ActualHeight;
                double left = (canvasWidth - imgWidth) / 2;
                double top = (canvasHeight - imgHeight) / 2;

                Canvas.SetLeft(image, left);
                Canvas.SetTop(image, top);
            }
        }

        private void SaveAs_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.Filter = "Bitmap Image (*.bmp)|*.bmp|JPEG Image (*.jpg)|*.jpg";
            if (saveFileDialog.ShowDialog() == true)
            {
                RenderTargetBitmap rtb = new RenderTargetBitmap((int)canvas.ActualWidth, (int)canvas.ActualHeight - 15, 96, 96, PixelFormats.Default);
                DrawingVisual dv = new DrawingVisual();

                using (DrawingContext dc = dv.RenderOpen())
                {
                    VisualBrush vb = new VisualBrush(canvas);
                    dc.DrawRectangle(vb, null, new Rect(new Point(), new Size(canvas.ActualWidth, canvas.ActualHeight - 20)));
                }

                rtb.Render(dv);

                BitmapEncoder encoder = null;
                switch (saveFileDialog.FilterIndex)
                {
                    case 1:
                        encoder = new BmpBitmapEncoder();
                        break;
                    case 2:
                        encoder = new JpegBitmapEncoder();
                        break;
                }

                if (encoder != null)
                {
                    encoder.Frames.Add(BitmapFrame.Create(rtb));
                    using (FileStream fileStream = File.Create(saveFileDialog.FileName))
                    {
                        encoder.Save(fileStream);
                    }
                }
            }
        }

        private void ComboBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            string colorString = ((ComboBoxItem)comboBoxColors.SelectedItem).Content.ToString();
            switch (colorString)
            {
                case "Black":
                    currentColor = System.Drawing.Color.Black;
                    break;
                case "Red":
                    currentColor = System.Drawing.Color.Red;
                    break;
                case "Blue":
                    currentColor = System.Drawing.Color.Blue;
                    break;
                case "Green":
                    currentColor = System.Drawing.Color.Green;
                    break;
                case "White":
                    currentColor = System.Drawing.Color.White;
                    break;
                case "Pink":
                    currentColor = System.Drawing.Color.Pink;
                    break;
            }
        }
    }
}