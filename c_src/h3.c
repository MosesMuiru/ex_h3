#include <h3/h3api.h>
#include <erl_nif.h>


static ERL_NIF_TERM
make_h3idx(ErlNifEnv * env, H3Index index)
{
    return enif_make_uint64(env, index);
}

static ERL_NIF_TERM lat_lng_to_cell(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[])
{
    LatLng latLng;
    int arity;
    const ERL_NIF_TERM *elems;

    int res;
    H3Index out;

    /* argv[0] = {lat, lng} */
    if (!enif_get_tuple(env, argv[0], &arity, &elems) || arity != 2)
        return enif_make_badarg(env);

    if (!enif_get_double(env, elems[0], &latLng.lat))
        return enif_make_badarg(env);

    if (!enif_get_double(env, elems[1], &latLng.lng))
        return enif_make_badarg(env);

    /* argv[1] = resolution */
    if (!enif_get_int(env, argv[1], &res))
        return enif_make_badarg(env);

    /* call H3 */
    H3Error err = latLngToCell(&latLng, res, &out);

    /* return result */
    return enif_make_uint64(env, out);
}

static ErlNifFunc nif_funcs[] = {
    {"lat_lng_to_cell", 2, lat_lng_to_cell}
};

ERL_NIF_INIT(Elixir.H3, nif_funcs, NULL, NULL, NULL, NULL);
