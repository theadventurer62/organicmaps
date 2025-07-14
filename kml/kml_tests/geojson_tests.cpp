#include "testing/testing.hpp"

#include "kml/serdes_common.hpp"
#include "kml/serdes_geojson.hpp"

#include "geometry/mercator.hpp"

#include "coding/file_reader.hpp"

#include "platform/platform.hpp"

namespace geojson_tests
{
static kml::FileData LoadGpxFromString(std::string_view content)
{
  TEST_NO_THROW(
  {
    kml::FileData dataFromText;
    kml::geojson::GeojsonParser(dataFromText).Parse(MemReader(content));
    return dataFromText;
  }, ());
}


UNIT_TEST(GeoJson_Parse_Basic)
{
  std::string_view constexpr input = R"({
  "type": "FeatureCollection",
  "features": [
    {
      "type": "Feature",
      "properties": {},
      "geometry": {
        "coordinates": [
          [
            14.949382505528291,
            8.16007148457335
          ],
          [
            26.888888114204264,
            9.708105796659268
          ],
          [
            37.54707497642465,
            6.884595662842159
          ]
        ],
        "type": "LineString"
      }
    },
    {
      "type": "Feature",
      "properties": {},
      "geometry": {
        "coordinates": [
          31.02177966625902,
          29.8310316130992
        ],
        "type": "Point"
      }
    }
  ]
})";

  kml::FileData data;
  kml::BookmarkData bookmarkData;
  bookmarkData.m_name[kml::kDefaultLang] = "Waypoint 1";
  bookmarkData.m_point = mercator::FromLatLon(42.81025, -1.65727);
  bookmarkData.m_customName[kml::kDefaultLang] = "Waypoint 1";
  bookmarkData.m_color = {kml::PredefinedColor::Red, 0};
  data.m_bookmarksData.emplace_back(std::move(bookmarkData));

  kml::FileData const dataFromText = LoadGpxFromString(input);

  TEST_EQUAL(dataFromText, data, ());
}

}  // namespace gpx_tests
