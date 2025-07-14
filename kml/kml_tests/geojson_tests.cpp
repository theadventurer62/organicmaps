#include "testing/testing.hpp"

#include "kml/serdes_common.hpp"
#include "kml/serdes_geojson.hpp"

#include "geometry/mercator.hpp"

#include "coding/file_reader.hpp"

#include "platform/platform.hpp"
#include "coding/serdes_json.hpp"

#include "base/exception.hpp"

namespace geojson_tests
{

/*
static kml::FileData LoadGeojsonFromString(std::string_view content)
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

  kml::FileData const dataFromText = LoadGeojsonFromString(input);

  //TEST_EQUAL(dataFromText, data, ());
}
*/

struct GeoJsonGeometry2 {
    std::vector<geometry::PointWithAltitude> m_coordinates;

    template <typename Visitor>
    void Visit(Visitor & visitor)
    {
        try {
            std::vector<double> coordData;
            visitor(coordData, "coordinates");
            if(coordData.size() != 2) {
                //ERROR!
            }
        }
        catch(RootException exc) {
            std::vector<std::pair<double, double>> polygonData;
            visitor(polygonData, "coordinates");
        }
    }

    template <typename Visitor>
    void Visit(Visitor & visitor) const
    {
        std::vector<double> coordData;
        visitor(coordData, "coordinates");
        // TODO
    }

    friend std::string DebugPrint(GeoJsonGeometry2 const & c)
    {
        DebugPrintVisitor visitor("GeoJsonGeometry");
        c.Visit(visitor);
        return visitor.ToString();
    }
};


UNIT_TEST(Geometry_parser)
{

    std::string_view constexpr input_case_way = R"(
    {
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
      ]
    })";

    std::string_view constexpr input_case_point = R"(
    {
      "coordinates": [
        31.02177966625902,
        29.8310316130992
      ]
    })";

    GeoJsonGeometry2 geometry;
    MemReader input(input_case_way);
    NonOwningReaderSource source(input);
    coding::DeserializerJson des(source);
    des(geometry);
}

}  // namespace gpx_tests
