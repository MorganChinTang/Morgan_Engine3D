#pragma once

namespace Engine3D::Graphics
{
    class Texture
    {
    public:
        static void UnbindPS(uint32_t slot);

        Texture() = default;
        virtual ~Texture();

        //Delay copy
        Texture(const Texture&) = delete;
        Texture& operator = (const Texture&) = delete;
        
        //Allow move
        Texture(Texture&& rhs) noexcept;
        Texture& operator = (Texture&& rhs) noexcept;

        virtual void Initialize(const std::filesystem::path& fileName);
        virtual void Terminate();

        void BindPS(uint32_t slot) const;
        void BindVS(uint32_t slot) const;

        void* GetRawData() const;


        uint32_t GetWidth() const;
        uint32_t GetHeight() const;
    protected:
        ID3D11ShaderResourceView* mShaderResourceView = nullptr;

        uint32_t mWidth = 0;
        uint32_t mHeight = 0;
    };
}