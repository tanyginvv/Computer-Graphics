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
            {}
        }

        private void AddSemiTransparentBackground()
        {
            ImageBrush imageBrush = new ImageBrush();
            imageBrush.ImageSource = new BitmapImage(new Uri("background.png", UriKind.Relative));

            grid.Background = imageBrush;
        }
        //так чтобы не тормозило
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
    }
}