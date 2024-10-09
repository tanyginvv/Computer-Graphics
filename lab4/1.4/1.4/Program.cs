using System;
using OpenTK.Windowing.Common;
using OpenTK.Windowing.Desktop;
using OpenTK.Mathematics;

namespace Icosahedron
{
    class Program
    {
        static void Main(string[] args)
        {
            var nativeWinSettings = new NativeWindowSettings()
            {
                ClientSize = new Vector2i(800, 800),
                Location = new Vector2i(30, 30),
                WindowBorder = WindowBorder.Resizable,
                WindowState = WindowState.Normal,
                Title = "Icosahedron",
                Flags = ContextFlags.Default,
                APIVersion = new Version(3, 3),
                Profile = ContextProfile.Compatability,
                API = ContextAPI.OpenGL,
                NumberOfSamples = 0
            };


            Window game = new Window(GameWindowSettings.Default, nativeWinSettings);
            game.Run();

        }
    }
}