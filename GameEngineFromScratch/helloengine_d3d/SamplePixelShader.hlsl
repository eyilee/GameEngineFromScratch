struct v2p {
	float4 position : SV_POSITION;
	float4 color	: COLOR;
};

float4 main(v2p input) : SV_TARGET
{
	return input.color;
}