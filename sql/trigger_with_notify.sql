-- TODO find out why new.name returns null
-- listen gas_channel; <~ need to listen to this channel in the app
create or replace function on_developer_update() returns trigger as
$$
begin
    -- log
    insert into logs (date, description)
    values ((select current_date), concat('developer ', lower(tg_op), ' ', new.name, old.name));
    -- notify
    notify gas_channel, 'developer update';
    return new;
end;
$$
    language plpgsql;

create trigger developer_update
    after insert or update or delete
    on developer
execute procedure on_developer_update();

create or replace function on_resourcetype_update() returns trigger as
$$
begin
    -- log
    insert into logs (date, description)
    values ((select current_date), concat('resourcetype ', lower(tg_op), ' ', new.name, old.name));
    -- notify
    notify gas_channel, 'resourcetype update';
    return new;
end;
$$
    language plpgsql;

create trigger resourcetype_update
    after insert or update or delete
    on resourcetype
execute procedure on_resourcetype_update();

create or replace function on_resource_update() returns trigger as
$$
begin
    -- log
    insert into logs (date, description)
    values ((select current_date), concat('resource ', lower(tg_op), ' ', new.name, old.name));
    -- notify
    notify gas_channel, 'resource update';
    return new;
end;
$$
    language plpgsql;

create trigger developer_update
    after insert or update or delete
    on resource
execute procedure on_resource_update();

create or replace function on_dependency_update() returns trigger as
$$
begin
    -- log
    insert into logs (date, description)
    values ((select current_date), concat('dependency ', lower(tg_op), ' ', new.name, old.name));
    -- notify
    notify gas_channel, 'dependency update';
    return new;
end;
$$
    language plpgsql;

create trigger dependency_update
    after insert or update or delete
    on dependency
execute procedure on_dependency_update();

create or replace function on_commit_update() returns trigger as
$$
begin
    -- log
    insert into logs (date, description)
    values ((select current_date), concat('commit ', lower(tg_op), ' ', new.name, old.name));
    -- notify
    notify gas_channel, 'commit update';
    return new;
end;
$$
    language plpgsql;

create trigger commit_update
    after insert or update or delete
    on commits
execute procedure on_commit_update();