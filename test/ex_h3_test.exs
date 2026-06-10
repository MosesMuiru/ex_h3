defmodule ExH3Test do
  use ExUnit.Case
  doctest ExH3

  setup_all do
    lat_lng = {37.3615593, -122.0553238}
    res = 10
    state = %{lat_lng: lat_lng, res: res, expected_index: ~c"8a283470d927fff"}
    {:ok, state}
  end

  test "converts lat/lng to H3 index", state do
    {:ok, index} = ExH3.lat_lng_to_cell(state[:lat_lng], state[:res])
    assert index == state[:expected_index]
  end

  test "converts H3 index to lat/lng", state do
    {:ok, {lat, lng}} = ExH3.cell_to_lat_lng(state[:expected_index])
    lat = trunc(lat)
    lng = trunc(lng)
    assert lat == state[:lat_lng] |> elem(0) |> trunc()
    assert lng == state[:lat_lng] |> elem(1) |> trunc()
  end

  test "converts cell to boundary", state do
    {expected_num_vertices, expected_vertices} =
      {6,
       [
         {37.36081881948407, -122.05466732876516},
         {37.361324418946275, -122.05402753508244},
         {37.361998502992506, -122.054231694951},
         {37.36216698379974, -122.055075655472},
         {37.36166138188755, -122.05571544499882},
         {37.36098730161813, -122.05551127816072}
       ]}

    {:ok, {num_vertices, vertices}} = ExH3.cell_to_boundary(state[:expected_index])

    assert expected_num_vertices == num_vertices
    assert expected_vertices == vertices
  end
end
