defmodule ExH3 do
  @on_load :__on_load__

  def __on_load__ do
    path = :filename.join(:code.priv_dir(:ex_h3), ~c"h3")
    :ok = :erlang.load_nif(path, 0)
  end

  def lat_lng_to_cell({_lat, _lng}, _res), do: :erlang.nif_error(:nif_not_loaded)

  def cell_to_lat_lng(_index), do: :erlang.nif_error(:nif_not_loaded)

  @moduledoc """
  Documentation for `ExH3`.
  """

  @doc """
  Hello world.

  ## Examples

      iex> ExH3.hello()
      :world

  """
  def hello do
    :world
  end
end
