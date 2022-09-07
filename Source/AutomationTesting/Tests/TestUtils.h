#pragma once


namespace UE::TEST
{

template <typename TestType, typename ExpectedType>
struct TestPayload
{
	TestType TestValue;
	ExpectedType ExpectedValue;
};

}
