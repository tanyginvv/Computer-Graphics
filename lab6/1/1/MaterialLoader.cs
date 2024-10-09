using Assimp;
using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;
using System.Drawing;
using System.Drawing.Imaging;

namespace ChessLab
{
    public class MaterialLoader
    {
        private List<int> textureIds = new();
        private bool texturesLoaded = false;

        public void LoadMaterialTextures(Material material)
        {
            int textureCount = material.GetMaterialTextureCount(TextureType.Diffuse);
            textureIds.Clear();
            textureIds.Capacity = textureCount;
            texturesLoaded = true;

            for (int i = 0; i < textureCount; i++)
            {
                if (material.GetMaterialTexture(TextureType.Diffuse, i, out TextureSlot textureSlot))
                {
                    if (File.Exists(textureSlot.FilePath))
                    {
                        int texId = GL.GenTexture();
                        GL.BindTexture(TextureTarget.Texture2D, texId);

                        using (Bitmap bitmap = new Bitmap(textureSlot.FilePath))
                        {
                            BitmapData data = bitmap.LockBits(
                                new Rectangle(0, 0, bitmap.Width, bitmap.Height),
                                ImageLockMode.ReadOnly,
                                System.Drawing.Imaging.PixelFormat.Format32bppArgb);

                            GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba,
                                          data.Width, data.Height, 0,
                                          OpenTK.Graphics.OpenGL.PixelFormat.Bgra, PixelType.UnsignedByte,
                                          data.Scan0);
                            bitmap.UnlockBits(data);
                        }

                        GL.GenerateMipmap(GenerateMipmapTarget.Texture2D);
                        GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapS, (int)Assimp.TextureWrapMode.Clamp);
                        GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureWrapT, (int)Assimp.TextureWrapMode.Clamp);
                        GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
                        GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);

                        textureIds.Add(texId);
                    }
                    else
                    {
                        texturesLoaded = false;
                        System.Console.WriteLine($"Texture file not found: {textureSlot.FilePath}");
                    }
                }
            }
        }

        public void ApplyMaterial(Material material, int index)
        {
            Color4 ambientColor = ConvertColor(material.ColorAmbient);
            Color4 diffuseColor = ConvertColor(material.ColorDiffuse);
            Color4 specularColor = ConvertColor(material.ColorSpecular);
            float shininess = material.Shininess;

            GL.Material(MaterialFace.FrontAndBack, MaterialParameter.Ambient, ambientColor);
            GL.Material(MaterialFace.FrontAndBack, MaterialParameter.Diffuse, diffuseColor);
            GL.Material(MaterialFace.FrontAndBack, MaterialParameter.Specular, specularColor);
            GL.Material(MaterialFace.FrontAndBack, MaterialParameter.Shininess, shininess);

            if (texturesLoaded && index < textureIds.Count && material.GetMaterialTexture(TextureType.Diffuse, index, out _))
            {
                GL.ActiveTexture(TextureUnit.Texture0);
                GL.BindTexture(TextureTarget.Texture2D, textureIds[index]);
            }
        }

        private Color4 ConvertColor(Color4D color4d)
        {
            return new Color4(color4d.R, color4d.G, color4d.B, color4d.A);
        }
    }
}
