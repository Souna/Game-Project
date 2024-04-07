//#include "SunNode.h"
//#include "Point.h"
//#include "Animation.h"
//
//
//namespace game
//{
//	// Collection of portals on a map. Draws and updates portals.
//	// Also contains methods for using portals and obtaining spawn points.
//	class MapPortals
//	{
//	public:
//		static void Init();
//
//		MapPortals(SunNode source, int32_t map_id);
//		MapPortals();
//
//		void Update(Point<int16_t> player_pos);
//		void Draw(Point<int16_t> view_pos, float inter) const;
//
//		Portal::WarpInfo Find_Warp_At(Point<int16_t> player_pos);
//
//		Point<int16_t> Get_Portal_By_Id(uint8_t id) const;
//		Point<int16_t> Get_Portal_By_Name(const std::string& name) const;
//
//	private:
//		static std::unordered_map<Portal::Type, Animation> animations_;
//
//		std::unordered_map<uint8_t, Portal> portals_by_id_;
//		std::unordered_map<std::string, uint8_t> portal_ids_by_name_;
//
//		static const int16_t WARPCD_ = 48;
//		int16_t cooldown_;
//	};
//}