#include <cassert>
#include <Network.h>

class Foo
{
public:
	Foo()
		: i(0)
		, b(false)
	{}

	void Serialize(Network::StreamWriter& writer)
	{
		writer.Write(i);
		writer.Write(b);
		writer.Write(name);
	}

	void Deserialize(Network::StreamReader& reader)
	{
		reader.Read(i);
		reader.Read(b);
		reader.Read(name);
	}

	int i;
	bool b;
	std::string name;
};

class Car
{
public:
	CLASS_ID(Car, 'CAR');

	int position{ 0 };
};

class Tesla : public Car
{
public:
	CLASS_ID(Tesla, 'TESL');

	Tesla() { position = 1; }
};

class Ferrari : public Car
{
public:
	CLASS_ID(Ferrari, 'FERR');

	Ferrari() { position = 2; }
};



int main()
{
	int outI = 42;
	float outF = 3.14f;
	double outD = 6.28;
	char outC = 'a';
	bool outB = true;
	std::string outStr = "test";

	Network::MemoryStream outStream;
	Network::StreamWriter writer(outStream);
	writer.Write(outI);
	writer.Write(outF);
	writer.Write(outD);
	writer.Write(outC);
	writer.Write(outB);
	writer.Write(outStr);

	int inI = 0;
	float inF = 0.0f;
	double inD = 0.0;
	char inC = '\0';
	bool inB = false;
	std::string inStr = "garbage";

	
	Network::MemoryStream inStream(outStream.GetData(), outStream.GetHead());
	Network::StreamReader reader(inStream);
	reader.Read(inI);
	reader.Read(inF);
	reader.Read(inD);
	reader.Read(inC);
	reader.Read(inB);
	reader.Read(inStr);

	
	assert(inI == outI);
	assert(inF == outF);
	assert(inD == outD);
	assert(inC == outC);
	assert(inB == outB);
	assert(inStr == outStr);

	Foo myFoo;
	myFoo.i = 77;
	myFoo.b = true;
	myFoo.name = "jackson";

	Network::MemoryStream fooOut;
	Network::StreamWriter fooWriter(fooOut);
	myFoo.Serialize(fooWriter);

	Network::MemoryStream fooIn(fooOut.GetData(), fooOut.GetHead());
	Network::StreamReader fooReader(fooIn);

	Foo urFoo;
	urFoo.Deserialize(fooReader);

	assert(myFoo.i == urFoo.i);
	assert(myFoo.b == urFoo.b);
	assert(myFoo.name == urFoo.name);

	////////////////////////////////////////////////////////////

	Network::ObjectFactory objectFactory;
	objectFactory.Register<Car>();
	objectFactory.Register<Tesla>();
	objectFactory.Register<Ferrari>();

	Car* c = (Car*)objectFactory.CreateInstance(Car::ClassId);
	Tesla* t = (Tesla*)objectFactory.CreateInstance(Tesla::ClassId);
	Ferrari* f = (Ferrari*)objectFactory.CreateInstance(Ferrari::ClassId);

	assert(c->position == 0);
	assert(t->position == 1);
	assert(f->position == 2);


	Network::LinkingContext linkingContext;
	int cid = linkingContext.GetNetworkId(c);
	int tid = linkingContext.GetNetworkId(t);
	int fid = linkingContext.GetNetworkId(f);

	assert(cid == Network::LinkingContext::sInvalidId);
	assert(tid == Network::LinkingContext::sInvalidId);
	assert(fid == Network::LinkingContext::sInvalidId);

	cid = linkingContext.Register(c);
	tid = linkingContext.Register(t);
	fid = linkingContext.Register(f);
	
	Car* cc = (Car*)linkingContext.GetInstance(cid);
	Tesla* tt = (Tesla*)linkingContext.GetInstance(tid);
	Ferrari* ff = (Ferrari*)linkingContext.GetInstance(fid);

	assert(c == cc);
	assert(t == tt);
	assert(f == ff);

	linkingContext.Unregister(c);
	linkingContext.Unregister(t);
	linkingContext.Unregister(f);

	Car* ccc = (Car*)linkingContext.GetInstance(cid);
	Car* ttt = (Tesla*)linkingContext.GetInstance(tid);
	Car* fff = (Ferrari*)linkingContext.GetInstance(fid);

	assert(ccc == nullptr);
	assert(ttt == nullptr);
	assert(fff == nullptr);

	cid = linkingContext.GetNetworkId(c);
	tid = linkingContext.GetNetworkId(t);
	fid = linkingContext.GetNetworkId(f);


	assert(cid == Network::LinkingContext::sInvalidId);
	assert(tid == Network::LinkingContext::sInvalidId);
	assert(fid == Network::LinkingContext::sInvalidId);

	return 0;
}

/*
// Snapshots
class World
{
public:
	void BuildSnapshot(Network::StreamWriter& writer)
	{
		// Tag packet, receiver should switch on this and decide how to handle packet
		writer.Write('SNAP');

		// Write objects
		for (auto go : mGameObjects)
		{
			// Write id
			writer.Write(mLinkingContext.GetNetworkId(go));

			// Write class
			writer.Write(go->GetClassId());

			// Write data
			go->Serialize(writer);
		}
	}

	void ApplySnapshot(Network::StreamReader& reader)
	{
		std::vector<GameObject*> replicatedObjects;
		while (reader.GetRemainingDataSize() > 0)
		{
			int networkId, classId;
			reader.Read(networkId);
			reader.Read(classId);

			GameObject* go = mLinkingContext.GetInstance(networkId);
			if (go == nullptr)
			{
				// Create object
				// Add to linking context
			}

			go->Deserialize(reader);
			replicatedObjects.push_back(go);
		}

		for every go in replicatedObjects not in mGameObjects
			// Remove from linking context
			// Destroy object

			mGameObjects = replicatedObjects;
	}

private:
	std::vector<GameObject*> mGameObjects;
	Network::LinkingContext mLinkingContext;
};
*/