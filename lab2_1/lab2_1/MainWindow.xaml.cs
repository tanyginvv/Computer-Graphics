using Microsoft.Win32;
using System;
using System.Windows;
using System.Windows.Controls;
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
            {}
        }

        private void AddSemiTransparentBackground()
        {
            Canvas canvas = new Canvas();
            canvas.Width = 1200;
            canvas.Height = 900;
            canvas.HorizontalAlignment = HorizontalAlignment.Stretch;
            canvas.VerticalAlignment = VerticalAlignment.Stretch;
            canvas.Margin = new Thickness(0);

            int numRows = 30;
            int numCols = 40;
            double cellWidth = canvas.Width / numCols;
            double cellHeight = canvas.Height / numRows;

            SolidColorBrush semiTransparentBrush = new SolidColorBrush(Colors.Transparent);
            semiTransparentBrush.Opacity = 0.1; 

            for (int row = 0; row < numRows; row++)
            {
                for (int col = 0; col < numCols; col++)
                {
                    Rectangle rectangle = new Rectangle();
                    rectangle.Width = cellWidth;
                    rectangle.Height = cellHeight;
                    rectangle.Fill = ((row + col) % 2 == 0) ? Brushes.Black : Brushes.White; 
                    rectangle.Opacity = 0.5; 
                    Canvas.SetLeft(rectangle, col * cellWidth); 
                    Canvas.SetTop(rectangle, row * cellHeight); 

                    canvas.Children.Add(rectangle);
                }
            }

            GridBackground.Children.Add(canvas);
        }

        private void ClearBackground()
        {
            Grid parentGrid = (Grid)ImageView.Parent;

            foreach (UIElement child in GridBackground.Children)
            {
                if (child is Canvas)
                {
                    GridBackground.Children.Remove(child);
                    break; 
                }
            }

            parentGrid.Background = Brushes.Transparent;
        }

    }
}