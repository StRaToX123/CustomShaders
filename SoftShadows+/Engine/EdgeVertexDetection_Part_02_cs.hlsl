



ByteAddressBuffer byteBuffer : register(t0); // matrix data,  192'th byte starts the vertexSize size float and the 195'th starts the Color float

Texture2D prevBackBuffer : register(t1);

RWByteAddressBuffer vertexData : register(u1);

SamplerState clampSampler : register(s0);



[numthreads(64, 1, 1)]
void main(uint3 groupId : SV_GroupID,
             uint groupIndex : SV_GroupIndex)
{
    float3 temp;
    float4 position;
    float4 inputPosition;

    temp.x = asfloat(byteBuffer.Load(192)); // temp.x now holds the vertexCount
    temp.y = floor(asfloat(byteBuffer.Load(196)) * 10.0f) / 10.0f; // temp.y now hols the color for this object


    if (((groupId.x * 64) + groupIndex) <= temp.x) // if the thread doesn't exceed the vertexCount
    {


        temp.x = ((groupId.x * 64) + groupIndex) * 36; // temp.x now holds the vertexData offset for the current vertex
        vertexData.Store(temp.x + 32, 0); // sets isEdgeVertex for this perticular vertex to 0
        inputPosition = float4(asfloat(vertexData.Load(temp.x)), asfloat(vertexData.Load(temp.x + 4)), asfloat(vertexData.Load(temp.x + 8)), 1.0f); // Loads the vertex model Position

   

        // Multiply Vertex modelSpace position with worldSpace matrix
        position.x = asfloat(byteBuffer.Load(0)) * inputPosition.x;
        temp.x = asfloat(byteBuffer.Load(4)) * inputPosition.y;
        position.x = position.x + temp.x;
        temp.x = asfloat(byteBuffer.Load(8)) * inputPosition.z;
        position.x = position.x + temp.x;
        temp.x = asfloat(byteBuffer.Load(12)) * inputPosition.w;
        position.x = position.x + temp.x;

        position.y = asfloat(byteBuffer.Load(16)) * inputPosition.x;
        temp.x = asfloat(byteBuffer.Load(20)) * inputPosition.y;
        position.y = position.y + temp.x;
        temp.x = asfloat(byteBuffer.Load(24)) * inputPosition.z;
        position.y = position.y + temp.x;
        temp.x = asfloat(byteBuffer.Load(28)) * inputPosition.w;
        position.y = position.y + temp.x;

        position.z = asfloat(byteBuffer.Load(32)) * inputPosition.x;
        temp.x = asfloat(byteBuffer.Load(36)) * inputPosition.y;
        position.z = position.z + temp.x;
        temp.x = asfloat(byteBuffer.Load(40)) * inputPosition.z;
        position.z = position.z + temp.x;
        temp.x = asfloat(byteBuffer.Load(44)) * inputPosition.w;
        position.z = position.z + temp.x;

        position.w = asfloat(byteBuffer.Load(48)) * inputPosition.x;
        temp.x = asfloat(byteBuffer.Load(52)) * inputPosition.y;
        position.w = position.w + temp.x;
        temp.x = asfloat(byteBuffer.Load(56)) * inputPosition.z;
        position.w = position.w + temp.x;
        temp.x = asfloat(byteBuffer.Load(60)) * inputPosition.w;
        position.w = position.w + temp.x;



    // Multiply Vertex worldSpace position with viewSpace matrix
        inputPosition.x = asfloat(byteBuffer.Load(64)) * position.x;
        temp.x = asfloat(byteBuffer.Load(68)) * position.y;
        inputPosition.x = inputPosition.x + temp.x;
        temp.x = asfloat(byteBuffer.Load(72)) * position.z;
        inputPosition.x = inputPosition.x + temp.x;
        temp.x = asfloat(byteBuffer.Load(76)) * position.w;
        inputPosition.x = inputPosition.x + temp.x;

        inputPosition.y = asfloat(byteBuffer.Load(80)) * position.x;
        temp.x = asfloat(byteBuffer.Load(84)) * position.y;
        inputPosition.y = inputPosition.y + temp.x;
        temp.x = asfloat(byteBuffer.Load(88)) * position.z;
        inputPosition.y = inputPosition.y + temp.x;
        temp.x = asfloat(byteBuffer.Load(92)) * position.w;
        inputPosition.y = inputPosition.y + temp.x;

        inputPosition.z = asfloat(byteBuffer.Load(96)) * position.x;
        temp.x = asfloat(byteBuffer.Load(100)) * position.y;
        inputPosition.z = inputPosition.z + temp.x;
        temp.x = asfloat(byteBuffer.Load(104)) * position.z;
        inputPosition.z = inputPosition.z + temp.x;
        temp.x = asfloat(byteBuffer.Load(108)) * position.w;
        inputPosition.z = inputPosition.z + temp.x;

        inputPosition.w = asfloat(byteBuffer.Load(112)) * position.x;
        temp.x = asfloat(byteBuffer.Load(116)) * position.y;
        inputPosition.w = inputPosition.w + temp.x;
        temp.x = asfloat(byteBuffer.Load(120)) * position.z;
        inputPosition.w = inputPosition.w + temp.x;
        temp.x = asfloat(byteBuffer.Load(124)) * position.w;
        inputPosition.w = inputPosition.w + temp.x;





    // Multiply Vertex viewSpace position with projectionSpace matrix
        position.x = asfloat(byteBuffer.Load(128)) * inputPosition.x;
        temp.x = asfloat(byteBuffer.Load(132)) * inputPosition.y;
        position.x = position.x + temp.x;
        temp.x = asfloat(byteBuffer.Load(136)) * inputPosition.z;
        position.x = position.x + temp.x;
        temp.x = asfloat(byteBuffer.Load(140)) * inputPosition.w;
        position.x = position.x + temp.x;

        position.y = asfloat(byteBuffer.Load(144)) * inputPosition.x;
        temp.x = asfloat(byteBuffer.Load(148)) * inputPosition.y;
        position.y = position.y + temp.x;
        temp.x = asfloat(byteBuffer.Load(152)) * inputPosition.z;
        position.y = position.y + temp.x;
        temp.x = asfloat(byteBuffer.Load(156)) * inputPosition.w;
        position.y = position.y + temp.x;

        position.z = asfloat(byteBuffer.Load(160)) * inputPosition.x;
        temp.x = asfloat(byteBuffer.Load(164)) * inputPosition.y;
        position.z = position.z + temp.x;
        temp.x = asfloat(byteBuffer.Load(168)) * inputPosition.z;
        position.z = position.z + temp.x;
        temp.x = asfloat(byteBuffer.Load(172)) * inputPosition.w;
        position.z = position.z + temp.x;

        position.w = asfloat(byteBuffer.Load(176)) * inputPosition.x;
        temp.x = asfloat(byteBuffer.Load(180)) * inputPosition.y;
        position.w = position.w + temp.x;
        temp.x = asfloat(byteBuffer.Load(184)) * inputPosition.z;
        position.w = position.w + temp.x;
        temp.x = asfloat(byteBuffer.Load(188)) * inputPosition.w;
        position.w = position.w + temp.x;


        

    // We will use inputPosition as destinations from now on since we don't need it anymore
        inputPosition.x = ((position.x / position.w) / 2.0f) + 0.5f;

        position.z = asfloat(bytebuffer.Load(200)); // position.z now holds the screenWidth

        inputPosition.x = inputPosition.x * position.z;
        inputPosition.x = round(inputPosition.x);
        inputPosition.z = floor(inputPosition.x);
        inputPosition.z = inputPosition.z - step(inputPosition.x, inputPosition.z);
        inputPosition.x = inputPosition.x / position.z;
        inputPosition.z = inputPosition.z / position.z;


        inputPosition.y = ((-position.y / position.w) / 2.0f) + 0.5f;

        temp.z = asfloat(bytebuffer.Load(204)); // temp.z now holds the screenHeight

        inputPosition.y = inputPosition.y * temp.z;
        inputPosition.y = round(inputPosition.y);
        inputPosition.y = inputPosition.y / temp.z;

        // we need to sample neighboring pixels based on our two coordinates
        // 1 : inputPosition.xy
        // 2 : inputPosition.zy

        // calculating texel sizes
        position.z = 1.0f / position.z;
        position.w = 1.0f / temp.z;
        

        // lets start with the first inputPosition.xy
        // middle-left pixel
        position.xy = inputPosition.xy + float2(-1.0f * position.z, 0.0f);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x; // reading the red channel only
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = 1.0f - asfloat(step(temp.z, 0.0f));

        // top-left
        position.xy = inputPosition.xy + float2(-1.0f * position.z, -1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

        // top-middle
        position.xy = inputPosition.xy + float2(0.0f, -1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

        // top-right
        position.xy = inputPosition.xy + float2(1.0f * position.z, -1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

        // middle-right
        position.xy = inputPosition.xy + float2(1.0f * position.z, 0.0f);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

        // bottom-right
        position.xy = inputPosition.xy + float2(1.0f * position.z, 1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

         // bottom-middle
        position.xy = inputPosition.xy + float2(0.0f, 1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

         // bottom-left
        position.xy = inputPosition.xy + float2(-1.0f * position.z, 1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));


        vertexData.Store((((groupId.x * 64) + groupIndex) * 36) + 32, step(1.0, temp.x));


        temp.x = 0.0f;


        // now we'll do it for inputPosition.zy
        // middle-left pixel
        position.xy = inputPosition.zy + float2(-1.0f * position.z, 0.0f);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x; // reading the red channel only
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = 1.0f - asfloat(step(temp.z, 0.0f));

        // top-left
        position.xy = inputPosition.zy + float2(-1.0f * position.z, -1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

        // top-middle
        position.xy = inputPosition.zy + float2(0.0f, -1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

        // top-right
        position.xy = inputPosition.zy + float2(1.0f * position.z, -1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

        // middle-right
        position.xy = inputPosition.zy + float2(1.0f * position.z, 0.0f);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

        // bottom-right
        position.xy = inputPosition.zy + float2(1.0f * position.z, 1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

         // bottom-middle
        position.xy = inputPosition.zy + float2(0.0f, 1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));

         // bottom-left
        position.xy = inputPosition.zy + float2(-1.0f * position.z, 1.0f * position.w);
        inputPosition.w = prevBackBuffer.SampleLevel(clampSampler, position.xy, 0.0f).x;
        inputPosition.w = floor(inputPosition.w * 10.0f) / 10.0f;
        temp.z = abs(inputPosition.w - temp.y);
        temp.x = temp.x + (1.0f - asfloat(step(temp.z, 0.0f)));


        position.x = step(1.0, temp.x);



        temp.x = (((groupId.x * 64) + groupIndex) * 36) + 32;
        position.y = asfloat(vertexData.Load(temp.x));

        if ((position.x > 0.0f) || (position.y > 0.0f))
        {
            vertexData.Store(temp.x, 1);
        }
        else
        {
            vertexData.Store(temp.x, 0);
        }
        
    }
       
    temp.x = 0.0f; // waste
}