struct Vertex{
	glm::vec3 mPosition;
	glm::vec3 mColor;

	Vertex(){}

	Vertex(const glm::vec3& position, const glm::vec3& color) 
		: mPosition(position), mColor(color)
	{
	}
};