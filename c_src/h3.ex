defmodule H3 do
  @on_load :load_nif

  def load_nif do
    :erlang.load_nif("./h3", 0)
  end

  def lat_lng_to_cell({_lat, _lng}, _res), do: :erlang.nif_error(:nif_not_loaded)
end
