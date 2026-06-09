defmodule ExH3 do
  @on_load :__on_load__

  def __on_load__ do
    path = :filename.join(:code.priv_dir(:ex_h3), ~c"h3")
    :ok = :erlang.load_nif(path, 0)
  end

  @type lat_lng_t :: {float(), float()}
  # h3_index is 64
  @type h3_index :: non_neg_integer()
  @type boundary_t :: %{
    num_verts: non_neg_integer(),
    verts: list(lat_lng_t())
  }

  def lat_lng_to_cell({_lat, _lng}, _res), do: :erlang.nif_error(:nif_not_loaded)

  def cell_to_lat_lng(_index), do: :erlang.nif_error(:nif_not_loaded)
  @spec cell_to_boundary(h3_index()) :: boundary_t()
  def cell_to_boundary(_index), do: :erlang.nif_error(:nif_not_loaded)

  @spec max_grid_disk_size(non_neg_integer()) :: non_neg_integer()
  def max_grid_disk_size(_k), do: :erlang.nif_error(:nif_not_loaded)

  @spec grid_disk_unsafe(h3_index(), non_neg_integer()) :: h3_index()
  def grid_disk_unsafe(_origin, _k), do: :erlang.nif_error(:nif_not_loaded)

  @spec grid_disk(h3_index(), non_neg_integer()) :: list(h3_index())
  def grid_disk(_origin, _k), do: :erlang.nif_error(:nif_not_loaded)

  @spec grid_disk_distances(h3_index(), non_neg_integer()) :: list({h3_index(), non_neg_integer()})
  def grid_disk_distances(_origin, _k), do: :erlang.nif_error(:nif_not_loaded)

  @spec max_grid_ring_size(non_neg_integer()) :: non_neg_integer()
  def max_grid_ring_size(_k), do: :erlang.nif_error(:nif_not_loaded)

  @spec grid_ring_unsafe(h3_index(), non_neg_integer()) :: list(h3_index())
  def grid_ring_unsafe(_origin, _k), do: :erlang.nif_error(:nif_not_loaded)

  @spec grid_ring(h3_index(), non_neg_integer()) :: list(h3_index())
  def grid_ring(_origin, _k), do: :erlang.nif_error(:nif_not_loaded)
end
