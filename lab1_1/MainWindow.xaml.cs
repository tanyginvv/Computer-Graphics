using System;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace lab1_1 
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

        }
        private void Grid_MouseUp(object sender, MouseButtonEventArgs e)
        {
            var grid = sender as Grid;
            if (grid != null)
            {
                foreach (var child in grid.Children)
                {
                    if (child is Grid innerGrid)
                    {
                        StartAnimation(innerGrid);
                    }
                }
            }
        }

        private async void StartAnimation(Grid grid)
        {
            double acceleration = 0.1;
            double moveDistance = 100;
            double maxSpeed = Math.Sqrt(2 * moveDistance * acceleration);
            double initialTopMargin = grid.Margin.Top; 
            bool moving = true;

            while (moving)
            {
                double targetY = initialTopMargin - moveDistance;
                double initialSpeedUp = maxSpeed;
                double speedUp = initialSpeedUp;

                while (grid.Margin.Top > targetY && speedUp > 0)
                {
                    speedUp -= acceleration;
                    grid.Margin = new Thickness(grid.Margin.Left, grid.Margin.Top - speedUp, grid.Margin.Right, grid.Margin.Bottom + speedUp);
                    await Task.Delay(16); 
                }

                double initialSpeedDown = 0; 
                double speedDown = initialSpeedDown;

                while (grid.Margin.Top < initialTopMargin && speedDown < maxSpeed)
                {
                    speedDown += acceleration;
                    grid.Margin = new Thickness(grid.Margin.Left, grid.Margin.Top + speedDown, grid.Margin.Right, grid.Margin.Bottom - speedDown);
                    await Task.Delay(16);
                }
 
                moving = true;
            }
        }
        // скругления букв доделать окно одного размера
    }
}