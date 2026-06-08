defmodule H3 do
  @on_load :load_nif

  @type lat_lng_t :: {float(), float()}
  # h3_index is 64
  @type h3_index :: non_neg_integer()
  @type boundary_t :: %{
    num_verts: non_neg_integer(),
    verts: list(lat_lng_t())
  }

  def load_nif do
    :erlang.load_nif("./h3", 0)
  end

  def lat_lng_to_cell({_lat, _lng}, _res), do: :erlang.nif_error(:nif_not_loaded)

  def cell_to_lat_lng(_index), do: :erlang.nif_error(:nif_not_loaded)

  @spec cell_to_boundary(h3_index()) :: boundary_t()
  def cell_to_boundary(_index), do: :erlang.nif_error(:nif_not_loaded)

end
