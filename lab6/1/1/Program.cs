using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;
using OpenTK.Mathematics;

namespace ChessLab
{
    class Program
    {
        static void Main(string[] args)
        {
            var nativeWinSettings = new NativeWindowSettings()
            {
                ClientSize = new Vector2i(1000, 1000),
                Location = new Vector2i(100, 100),
                WindowBorder = WindowBorder.Resizable,
                WindowState = WindowState.Normal,
                Title = "ChessLab",
                Flags = ContextFlags.Default,
                APIVersion = new Version(3, 3),
                Profile = ContextProfile.Compatability,
                API = ContextAPI.OpenGL,
                NumberOfSamples = 0
            };


            ChessLab.ChessWindow game = new (GameWindowSettings.Default, nativeWinSettings);
            game.Run();

        }
    }
}