#pragma once


namespace Phoenix{

	enum class ShaderDataType : uint8_t{
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};


	//////////////////////////////////////////////////////////////////////////////////////



	static uint32_t ShaderDataType_size(ShaderDataType type){
		switch(type){
			case ShaderDataType::None    : 	return 0;
			case ShaderDataType::Float   : 	return 4;
			case ShaderDataType::Float2  : 	return 4 * 2;
			case ShaderDataType::Float3  : 	return 4 * 3;
			case ShaderDataType::Float4  : 	return 4 * 4;
			case ShaderDataType::Mat3    : 	return 4 * 3 * 3;
			case ShaderDataType::Mat4    : 	return 4 * 4 * 4;
			case ShaderDataType::Int     : 	return 4;
			case ShaderDataType::Int2    : 	return 4 * 2;
			case ShaderDataType::Int3    : 	return 4 * 3;
			case ShaderDataType::Int4    : 	return 4 * 4;
			case ShaderDataType::Bool    : 	return 1;
		}

		ph_internal_error("Unknown ShaderDataType: " << (int)type);
		trace();
		return 0;
	};


	//////////////////////////////////////////////////////////////////////////////////////



	struct BufferElement{
		std::string name;
		ShaderDataType type;
		uint32_t offset;
		uint32_t size;
		bool normalized;

		BufferElement(){};

		BufferElement(ShaderDataType _type, const std::string& _name, bool normalized = false)
			: name(_name), type(_type), size(ShaderDataType_size(_type)), offset(0), normalized(normalized) {

		}


		uint32_t get_component_count() const {
			switch(type){
				case ShaderDataType::None    : 	return 0;
				case ShaderDataType::Float   : 	return 1;
				case ShaderDataType::Float2  : 	return 2;
				case ShaderDataType::Float3  : 	return 3;
				case ShaderDataType::Float4  : 	return 4;
				case ShaderDataType::Mat3    : 	return 3 * 3;
				case ShaderDataType::Mat4    : 	return 4 * 4;
				case ShaderDataType::Int     : 	return 1;
				case ShaderDataType::Int2    : 	return 1;
				case ShaderDataType::Int3    : 	return 2;
				case ShaderDataType::Int4    : 	return 3;
				case ShaderDataType::Bool    : 	return 1;
			}

			ph_internal_error("Unknown ShaderDataType: " << (int)type);
			trace();
			return 0;
		}
	};

	//////////////////////////////////////////////////////////////////////////////////////


	class BufferLayout{
		private:
			std::vector<BufferElement> elements;
			uint32_t stride = 0;
	
		public:
			BufferLayout(){};
			BufferLayout(const std::initializer_list<BufferElement> _elements)
				: elements(_elements){

				calculate_offsets_and_stride();

			}

			inline uint32_t get_stride() const { return stride; }
			inline const std::vector<BufferElement>& getElements() const { return elements; }

			std::vector<BufferElement>::iterator begin() { return elements.begin(); }
			std::vector<BufferElement>::iterator end() { return elements.end(); }

			std::vector<BufferElement>::const_iterator begin() const { return elements.begin(); }
			std::vector<BufferElement>::const_iterator end() const { return elements.end(); }

		private:
			void calculate_offsets_and_stride(){
				uint32_t offset = 0;
				stride = 0;
				for(auto& element : elements){
					element.offset = offset;
					offset += element.size;
					stride += element.size;
				}
			};
			
	};


	//////////////////////////////////////////////////////////////////////////////////////

	
	class VertexBuffer{
		public:
			virtual ~VertexBuffer(){};

			virtual void bind() const = 0;
			virtual void unbind() const = 0;

			virtual void setLayout(const BufferLayout& layout) = 0;
			virtual const BufferLayout& getLayout() = 0;

			static VertexBuffer* create(float* verticies, uint32_t size);
			
	};


	//////////////////////////////////////////////////////////////////////////////////////


	class IndexBuffer{
		public:
			virtual ~IndexBuffer(){};

			virtual void bind() const = 0;
			virtual void unbind() const = 0;

			virtual uint32_t getCount() const = 0;

			static IndexBuffer* create(uint32_t* indicies, uint32_t count);
	};


}
