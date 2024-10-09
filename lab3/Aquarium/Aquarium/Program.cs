using OpenTK.Mathematics;
using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;

namespace Aquarium
{
    class Program
    {
        static void Main(string[] args)
        {

            var nativeWinSettings = new NativeWindowSettings()
            {
                Size = new Vector2i(600, 600),
                Location = new Vector2i(30, 30),
                WindowBorder = WindowBorder.Resizable,
                WindowState = WindowState.Normal,
                Title = "Aquarium",
                Flags = ContextFlags.Default,
                APIVersion = new Version(3, 3),
                Profile = ContextProfile.Compatability,
                API = ContextAPI.OpenGL,
                NumberOfSamples = 0
            };

            using (Game game = new Game(GameWindowSettings.Default, nativeWinSettings))
            {
                game.Run();
            }
        }
    }
}
