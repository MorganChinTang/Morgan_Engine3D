#pragma once

namespace Engine3D::Graphics
{
    class Texture
    {
    public:
        static void UnbindPS(uint32_t slot);

        Texture() = default;
        ~Texture();

        //Delay copy
        Texture(const Texture&) = delete;
        Texture& operator = (const Texture&) = delete;
        
        //Allow move
        Texture(Texture&& rhs) noexcept;
        Texture& operator = (Texture&& rhs) noexcept;

        void Initialize(const std::filesystem::path& fileName);
        void Terminate();

        void BindPS(uint32_t slot) const;
        void BindVS(uint32_t slot) const;

        void* GetRawData() const;

    private:
        ID3D11ShaderResourceView* mShaderResourceView = nullptr;
    };
}