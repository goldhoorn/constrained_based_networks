#include "state_machines.hpp"
void load_constraints(){
using namespace constrained_based_networks;
auto pool = Pool::getInstance();
	{
	auto c = pool->getComponent("AuvControl::DepthFusionCmp");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("z","DepthReader::Task");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("AuvControl::DepthFusionCmp");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("ori","PoseAuv::IKFOrientationEstimatorCmp");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("LowLevel::Cmp");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("z","AuvControl::DepthFusionCmp");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("Localization::DeadReckoning");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("hb","Hbridge::SensorReader");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("Localization::ParticleDetector");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("hb","Hbridge::SensorReader");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("Localization::ParticleDetector");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("ori","AuvControl::DepthFusionCmp");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("PoseAuv::PoseEstimatorCmp");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("ori","AuvControl::DepthFusionCmp");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("Pipeline::Detector");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("camera","CameraProsilica::Task");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("Pipeline::Follower");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("controller","Pipeline::Detector");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("Pipeline::Follower");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("controller_local","Pipeline::Detector");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("PoseAuv::PoseEstimatorBlindCmp");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("depth","AuvControl::DepthFusionCmp");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("PoseAuv::PoseEstimatorBlindCmp");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("ori","AuvControl::DepthFusionCmp");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("Wall::Follower");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("controller","Wall::Detector");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("Wall::Follower");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("controller_local","Wall::Detector");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
	{
	auto c = pool->getComponent("Wall::Detector");
	if(auto cmp = dynamic_cast<Composition*>(c)){
		cmp->addConstraint("orientation_with_z","AuvControl::DepthFusionCmp");
	}else{ std::cerr << "FATAL cannot cast to composition" << std::endl;}
	}
load_state_machines();
}
