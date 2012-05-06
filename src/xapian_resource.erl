-module(xapian_resource).
-export([
    bool_weight/1, 
    bm25_weight/2, 
    bm25_weight/6, 
    trad_weight/1, 
    trad_weight/2]).

-import(xapian_common, [append_double/2]).

-compile({parse_transform, seqbind}).

-include_lib("xapian/include/xapian.hrl").


bool_weight(Server) ->
    xapian_drv:internal_create_resource(Server, bool_weight).


bm25_weight(Server, 
    #x_bm25_weight{k1=K1, k2=K2, l3=K3, b=B, min_normlen=MinNormLen}) ->
    bm25_weight(Server, K1, K2, K3, B, MinNormLen).

bm25_weight(Server, K1, K2, K3, B, MinNormLen) ->
    GenFn = 
        fun(_Register) ->
            Bin@ = <<>>,
            Bin@ = append_double(K1, Bin@),
            Bin@ = append_double(K2, Bin@),
            Bin@ = append_double(K3, Bin@),
            Bin@ = append_double(B, Bin@),
            Bin@ = append_double(MinNormLen, Bin@),
            {ok, Bin@}
        end,
    xapian_drv:internal_create_resource(Server, bm25_weight, GenFn).
    

trad_weight(Server) ->
    trad_weight(Server, 1).

trad_weight(Server, K) ->
    GenFn = 
        fun(_Register) ->
            {ok, append_double(K, <<>>)}
        end,
    xapian_drv:internal_create_resource(Server, trad_weight, GenFn).




%% ------------------------------------------------------------------
%% Tests
%% ------------------------------------------------------------------

-ifdef(TEST).
-include_lib("eunit/include/eunit.hrl").
-define(DRV, xapian_drv).
-define(RES, ?MODULE).

testdb_path(Name) -> 
	TestDir = filename:join(code:priv_dir(xapian), test_db),
	file:make_dir(TestDir),
	filename:join(TestDir, Name).


cases_test_() ->
    Cases = 
    [ fun bool_weight_case/1
    , fun bm25_weight_case/1
    , fun trad_weight_case/1
    ],
    Server = resource_setup(),
    %% One setup for each test
    {setup, 
        fun() -> Server end, 
        fun resource_clean/1,
        [Case(Server) || Case <- Cases]}.


resource_setup() ->
    % Open test
    Path = testdb_path(resource_creation),
    Params = [write, create, overwrite],
    {ok, Server} = ?DRV:open(Path, Params),
    Server.
    

resource_clean(Server) ->
    ?DRV:close(Server).


bool_weight_case(Server) ->
    Case = fun() ->
        ResourceId = ?RES:bool_weight(Server),
        io:format(user, "Xapian::BoolWeight ~p~n", [ResourceId])
        end,
    {"Check creation of Xapian::BoolWeight", Case}.


bm25_weight_case(Server) ->
    Case = fun() ->
        ResourceId = ?RES:bm25_weight(Server, #x_bm25_weight{}),
        io:format(user, "Xapian::BM25Weight ~p~n", [ResourceId])
        end,
    {"Check creation of Xapian::BM25Weight", Case}.


trad_weight_case(Server) ->
    Case = fun() ->
        ResourceId = ?RES:trad_weight(Server),
        io:format(user, "Xapian::TradWeight ~p~n", [ResourceId])
        end,
    {"Check creation of Xapian::TradWeight", Case}.

-endif.