using Microsoft.Win32;
using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace lab2_1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void MenuItem_Open_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Image Files (*.png;*.jpg;*.bmp)|*.png;*.jpg;*.bmp|All files (*.*)|*.*";

            if (openFileDialog.ShowDialog() == true)
            {
                string selectedImagePath = openFileDialog.FileName;
                DisplaySelectedImage(selectedImagePath);
            }
        }

        private void DisplaySelectedImage(string imagePath)
        {
            try
            {
                BitmapImage bitmap = new BitmapImage(new Uri(imagePath));
                ImageView.Source = bitmap;
                ClearBackground();

                if (System.IO.Path.GetExtension(imagePath).ToLower() == ".png")
                {
                    AddSemiTransparentBackground();
                }
                else
                {
                    ClearBackground();
                }
            }
            catch (Exception)
            { }
        }
        //размеры клеток постоянные заливка
        private void AddSemiTransparentBackground()
        {
            ImageBrush imageBrush = new ImageBrush();
            imageBrush.ImageSource = new BitmapImage(new Uri("background.png", UriKind.Relative));

            grid.Background = imageBrush;
        }
        private void ClearBackground()
        {
            Grid parentGrid = (Grid)ImageView.Parent;

            foreach (UIElement child in grid.Children)
            {
                if (child is Canvas)
                {
                    grid.Children.Remove(child);
                    break;
                }
            }

            parentGrid.Background = Brushes.Transparent;
        }

        private bool isDragging = false;
        private Point startPoint;
        private double startLeft;
        private double startTop;

        private void Grid_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (e.Source == ImageView || e.Source == BackgroundImage)
            {
                isDragging = true;
                startPoint = e.GetPosition(viewbox);
                var transform = grid.RenderTransform as TranslateTransform;
                if (transform != null)
                {
                    startLeft = transform.X;
                    startTop = transform.Y;
                }
                else
                {
                    transform = new TranslateTransform();
                    grid.RenderTransform = transform;
                    startLeft = 0;
                    startTop = 0;
                }
                grid.CaptureMouse();
            }
        }

        private void Grid_MouseMove(object sender, MouseEventArgs e)
        {
            if (isDragging)
            {
                Point mousePos = e.GetPosition(viewbox);
                double deltaX = mousePos.X - startPoint.X;
                double deltaY = mousePos.Y - startPoint.Y;

                var transform = grid.RenderTransform as TranslateTransform;
                if (transform != null)
                {
                    double newX = startLeft + deltaX;
                    double newY = startTop + deltaY;

                    transform.X = newX;
                    transform.Y = newY;
                }
            }
        }
        private void Grid_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (isDragging)
            {
                isDragging = false;
                grid.ReleaseMouseCapture();
            }
        }
    }
}